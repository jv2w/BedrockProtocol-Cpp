<?php

/**
 * Line-oriented transpiler for decodePayload()/encodePayload() bodies.
 *
 * A survey of all 229 packets found 2563 statement lines falling into 356 distinct shapes, of which
 * the top ~15 cover the large majority. This transpiler handles those shapes and leaves anything it
 * does not recognise as a `// PORT-TODO:` marker carrying the original PHP line, so that gaps are
 * visible rather than silently mistranslated.
 */

declare(strict_types=1);

namespace BedrockProtocolPort;

final class TranspileResult
{
    /** @param string[] $lines @param string[] $todos */
    public function __construct(public array $lines, public array $todos)
    {
    }
}

final class Transpiler
{
    /** @param array<string, string> $uses short PHP class name => fully-qualified name */
    /** @param array<string, string> $fieldTypes property name => C++ type */
    public function __construct(
        private array $uses,
        private array $fieldTypes,
        private string $className,
        private ?string $parentClass = null,
    ) {
    }

    /** @var string[] */
    private array $todos = [];

    public function transpile(string $phpBody, bool $isDecode): TranspileResult
    {
        $this->todos = [];
        $out = [];
        $indent = 1;  // one level inside the method body

        // Block stack, so that switch bodies can be indented the C++ way (statements one level below
        // their case label) while PHP keeps both at the same level.
        $stack = [];

        $bodyLines = explode("\n", trim($phpBody, "\n"));
        foreach ($bodyLines as $raw) {
            $trimmed = trim(rtrim($raw));

            if ($trimmed === '') {
                $out[] = '';
                continue;
            }

            $isCaseLabel = (bool)preg_match('/^(?:case\b.*:|default:)$/', $trimmed);
            $isClose = str_starts_with($trimmed, '}');
            $isOpen = str_ends_with($trimmed, '{');
            $topIndex = count($stack) - 1;

            // A new case label, or the end of the switch, terminates the previous case body.
            if (($isCaseLabel || $isClose) && $topIndex >= 0 && $stack[$topIndex]['kind'] === 'switch' && $stack[$topIndex]['caseOpen']) {
                --$indent;
                $stack[$topIndex]['caseOpen'] = false;
            }
            if ($isClose) {
                --$indent;
            }

            $pad = str_repeat(' ', max(0, $indent) * 4);

            if (str_starts_with($trimmed, '//')) {
                $out[] = $pad . $trimmed;
                continue;
            }

            foreach (explode("\n", $this->translateStatement($trimmed, $isDecode)) as $piece) {
                $out[] = $piece === '' ? '' : $pad . $piece;
            }

            if ($isClose) {
                array_pop($stack);
            }
            if ($isCaseLabel) {
                $topIndex = count($stack) - 1;
                if ($topIndex >= 0 && $stack[$topIndex]['kind'] === 'switch') {
                    ++$indent;
                    $stack[$topIndex]['caseOpen'] = true;
                }
            }
            if ($isOpen) {
                $stack[] = ['kind' => str_starts_with($trimmed, 'switch(') ? 'switch' : 'block', 'caseOpen' => false];
                ++$indent;
            }
        }

        return new TranspileResult($out, $this->todos);
    }

    private function translateStatement(string $s, bool $isDecode): string
    {
        // Split off a trailing comment so it survives untouched.
        $comment = '';
        if (preg_match('/^(.*?)(\s*\/\/.*)$/', $s, $m) && !str_contains($m[1], '"')) {
            $s = rtrim($m[1]);
            $comment = $m[2];
        }

        $s = $this->rewriteInterpolatedStrings($s);

        // Whatever interpolation could not be rewritten still needs a human, as does PHP's `.`
        // concatenation operator, which has no C++ equivalent for mixed operand types.
        $needsReview = preg_match('/"[^"]*\$[^"]*"/', $s) === 1 || preg_match('/"\s*\.\s*|\s*\.\s*"/', $s) === 1;

        $result = $this->translateCore($s, $isDecode);
        if ($needsReview && !str_contains($result, 'PORT-TODO')) {
            $this->todos[] = $s . '  (string interpolation/concatenation)';
            $result .= '  // PORT-TODO: rewrite the message text';
        }
        return $result . $comment;
    }

    private function translateCore(string $s, bool $isDecode): string
    {
        // ---- Block delimiters -------------------------------------------------
        if ($s === '}') {
            return '}';
        }
        if ($s === '}else{') {
            return "}\nelse {";
        }
        if (preg_match('/^\}elseif\((.*)\)\{$/', $s, $m)) {
            return "}\nelse if (" . $this->expr($m[1]) . ') {';
        }
        if ($s === '};') {
            return '};';
        }
        if ($s === 'break;' || $s === 'continue;') {
            return $s;
        }

        // ---- Control flow -----------------------------------------------------
        if (preg_match('/^if\((.*)\)\{$/', $s, $m)) {
            return 'if (' . $this->expr($m[1]) . ') {';
        }
        if (preg_match('/^while\((.*)\)\{$/', $s, $m)) {
            return 'while (' . $this->expr($m[1]) . ') {';
        }
        if (preg_match('/^switch\((.*)\)\{$/', $s, $m)) {
            return 'switch (' . $this->expr($m[1]) . ') {';
        }
        if (preg_match('/^case (.*):$/', $s, $m)) {
            return 'case ' . $this->expr($m[1]) . ':';
        }
        if ($s === 'default:') {
            return 'default:';
        }

        // for($i = 0, $count = Reader($in); $i < $count; ++$i){
        if (preg_match('/^for\(\$(\w+) = 0, \$(\w+) = (.+?); \$\1 < \$\2; \+?\+?\$?\1\+?\+?\)\{$/', $s, $m)) {
            $counterType = $this->inferReaderType($m[3]) ?? 'std::uint32_t';
            return sprintf('for (%s %s = 0, %s = %s; %s < %s; ++%s) {', $counterType, $m[1], $m[2], $this->expr($m[3]), $m[1], $m[2], $m[1]);
        }
        // for($i = 0; $i < $count; ++$i){
        if (preg_match('/^for\(\$(\w+) = 0; \$\1 < (.+?); \+?\+?\$?\1\+?\+?\)\{$/', $s, $m)) {
            return sprintf('for (std::uint32_t %s = 0; %s < %s; ++%s) {', $m[1], $m[1], $this->expr($m[2]), $m[1]);
        }
        // foreach($this->list as $k => $v){
        if (preg_match('/^foreach\((.+?) as \$(\w+) => \$(\w+)\)\{$/', $s, $m)) {
            return sprintf('for (const auto &[%s, %s] : %s) {', $m[2], $m[3], $this->expr($m[1]));
        }
        // foreach($this->list as $v){
        if (preg_match('/^foreach\((.+?) as \$(\w+)\)\{$/', $s, $m)) {
            return sprintf('for (const auto &%s : %s) {', $m[2], $this->expr($m[1]));
        }

        // ---- Throws -----------------------------------------------------------
        if (preg_match('/^throw new \\\\?(\w+)\((.*)\);$/', $s, $m)) {
            $exception = match ($m[1]) {
                'LogicException' => 'std::logic_error',
                'InvalidArgumentException' => 'std::invalid_argument',
                'RuntimeException' => 'std::runtime_error',
                default => $m[1],
            };
            return sprintf('throw %s(%s);', $exception, $this->expr($m[2]));
        }

        // ---- Assignments ------------------------------------------------------
        // $this->list = [];
        if (preg_match('/^\$this->(\w+) = \[\];$/', $s, $m)) {
            return $m[1] . '.clear();';
        }
        // $this->list[] = expr;
        if (preg_match('/^\$this->(\w+)\[\] = (.+);$/', $s, $m)) {
            return sprintf('%s.push_back(%s);', $m[1], $this->expr($m[2]));
        }
        // $this->map[expr] = expr;
        if (preg_match('/^\$this->(\w+)\[(.+?)\] = (.+);$/', $s, $m)) {
            return sprintf('%s.emplace_back(%s, %s);', $m[1], $this->expr($m[2]), $this->expr($m[3]));
        }
        // $this->field = expr;
        if (preg_match('/^\$this->(\w+) = (.+);$/', $s, $m)) {
            return sprintf('%s = %s;', $m[1], $this->floatDivision($m[1], $this->expr($m[2])));
        }
        // $local[] = expr;
        if (preg_match('/^\$(\w+)\[\] = (.+);$/', $s, $m)) {
            return sprintf('%s.push_back(%s);', $m[1], $this->expr($m[2]));
        }
        // $local = [];
        if (preg_match('/^\$(\w+) = \[\];$/', $s, $m)) {
            $this->todos[] = $s . '  (element type of the local container must be filled in)';
            return sprintf('// PORT-TODO: %s', $s);
        }
        // $local = expr;   (first assignment -> declaration)
        if (preg_match('/^\$(\w+) = (.+);$/', $s, $m)) {
            return sprintf('const auto %s = %s;', $m[1], $this->expr($m[2]));
        }

        // ---- Bare calls -------------------------------------------------------
        if (preg_match('/^(.+);$/', $s, $m) && str_contains($s, '(')) {
            return $this->expr($m[1]) . ';';
        }

        $this->todos[] = $s;
        return '// PORT-TODO: ' . $s;
    }

    /**
     * Forces a divisor literal to floating point when the assigned field is a float.
     *
     * PHP's / always produces a float, so `$this->x = $pos->getX() / 8;` is exact there. Emitted
     * verbatim into C++ the same expression is integer division on two ints, which truncates and
     * quietly snaps the value - PlaySoundPacket lost all sub-block sound precision this way, and the
     * defect was invisible until the deep round-trip suite compared the re-encoded bytes.
     *
     * Only integer literal divisors are rewritten. A divisor that is itself a float literal is
     * already correct, and one that is an expression is left alone rather than guessed at.
     */
    private function floatDivision(string $field, string $cppExpr): string
    {
        if (($this->fieldTypes[$field] ?? '') !== 'float') {
            return $cppExpr;
        }
        return preg_replace('~/\s*(\d+)(?![\d.eEfF])~', '/ $1.0F', $cppExpr);
    }

    /** Translates a single PHP expression to C++. Exposed for the factory-method generator. */
    public function translateExpression(string $e): string
    {
        return $this->expr($e);
    }

    /** Translates a PHP expression to C++ using the naming and namespace rules of PORTING.md. */
    private function expr(string $e): string
    {
        $e = trim($e);

        // parent::method(...) -> BaseClass::method(...)
        if ($this->parentClass !== null) {
            $e = str_replace('parent::', $this->parentClass . '::', $e);
        }

        $e = $this->rewriteFirstClassCallables($e);

        // PHP instantiates with `new X(...)`; C++ constructs a value, and the class name needs the
        // same namespace qualification a static call would get.
        $e = preg_replace_callback('/\bnew\s+([A-Z]\w*)/', function (array $m): string {
            return $this->mapConstructorName($m[1]);
        }, $e) ?? $e;

        // A packet embedded in another packet calls decodePayload/encodePayload on it. Those are
        // protected, and unlike PHP, C++ does not let a sibling class reach them - so route through
        // the public body entry points instead. The bytes are identical.
        $e = preg_replace('/->decodePayload\(/', '->decodeBody(', $e) ?? $e;
        $e = preg_replace('/->encodePayload\(/', '->encodeBody(', $e) ?? $e;

        // count($x) -> static_cast<std::uint32_t>(x.size())
        $e = preg_replace_callback('/\bcount\((.+?)\)/', function (array $m): string {
            return 'static_cast<std::uint32_t>(' . $this->expr($m[1]) . '.size())';
        }, $e) ?? $e;

        // Reader/writer handles.
        $e = preg_replace('/\$in\b/', 'in', $e) ?? $e;
        $e = preg_replace('/\$out\b/', 'out', $e) ?? $e;

        // Static call targets.
        $e = preg_replace_callback('/(?<![\w:])([A-Z]\w*)::/', function (array $m): string {
            return $this->mapClassName($m[1]) . '::';
        }, $e) ?? $e;

        // self::CONST / static::CONST -> CONST
        $e = preg_replace('/\b(?:self|static)::/', '', $e) ?? $e;

        // Property and local variable access.
        $e = preg_replace('/\$this->(\w+)/', '$1', $e) ?? $e;
        $e = preg_replace('/\$(\w+)/', '$1', $e) ?? $e;

        // Comparison and boolean operators.
        $e = str_replace(['===', '!==', ' and ', ' or ', ' xor '], ['==', '!=', ' && ', ' || ', ' ^ '], $e);

        // Nullable values are std::optional in this port (PORTING.md 3.2), so a bare `null` literal
        // passed as an argument or assigned to a field means "no value".
        $e = preg_replace('/(?<![\w:>])null(?![\w:])/', 'std::nullopt', $e) ?? $e;

        // pocketmine/binaryutils Limits constants map onto the <cstdint> macros.
        $e = preg_replace('/\bLimits::(U?INT(?:8|16|32|64))_(MIN|MAX)\b/', '$1_$2', $e) ?? $e;

        // PHP arrow on a value object becomes a dot; unique_ptr members need manual review.
        $e = preg_replace('/(?<=[\w\)\]])->/', '.', $e) ?? $e;

        // String concatenation used in exception messages.
        if (str_contains($e, ' . ')) {
            $e = str_replace(' . ', ' + ', $e);
        }

        return $e;
    }

    /**
     * Rewrites PHP string interpolation into C++ concatenation.
     *
     * "Unknown book edit type $this->type!" becomes
     * "Unknown book edit type " + std::to_string(type) + "!". Numeric fields go through
     * std::to_string; string fields are concatenated directly.
     */
    private function rewriteInterpolatedStrings(string $s): string
    {
        return preg_replace_callback('/"([^"]*\$this->\w+[^"]*)"/', function (array $m): string {
            $parts = preg_split('/\$this->(\w+)/', $m[1], -1, PREG_SPLIT_DELIM_CAPTURE);
            $pieces = [];
            foreach ($parts as $i => $part) {
                if ($i % 2 === 0) {
                    if ($part !== '') {
                        $pieces[] = '"' . $part . '"';
                    }
                    continue;
                }
                $type = $this->fieldTypes[$part] ?? '';
                $isNumeric = $type === 'bool' || $type === 'float' || $type === 'double'
                    || str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint');
                $pieces[] = $isNumeric ? "std::to_string({$part})" : $part;
            }
            // The first operand has to be a std::string for operator+ to chain over string literals.
            if ($pieces !== [] && str_starts_with($pieces[0], '"')) {
                $pieces[0] = 'std::string(' . $pieces[0] . ')';
            }
            return implode(' + ', $pieces);
        }, $s) ?? $s;
    }

    /**
     * Rewrites PHP's first-class callable syntax into C++ lambdas.
     *
     * PHP writes `CommonTypes::readOptional($in, CommonTypes::getString(...))`; C++ has no equivalent
     * of `f(...)`, so the callable has to become a lambda with the shape the template expects. Reader
     * callables take the reader; writer callables take the writer and the value.
     */
    private function rewriteFirstClassCallables(string $e): string
    {
        // readOptional / any reader-shaped callable argument.
        $e = preg_replace_callback(
            '/readOptional\(\$?(\w+),\s*([\w:]+)::(\w+)\(\.\.\.\)\)/',
            function (array $m): string {
                return sprintf(
                    'readOptional(%s, [](encoding::ByteBufferReader &reader) { return %s::%s(reader); })',
                    $m[1],
                    $this->mapClassName($m[2]),
                    $m[3]
                );
            },
            $e
        ) ?? $e;

        // writeOptional(out, value, Writer::put(...))
        $e = preg_replace_callback(
            '/writeOptional\(\$?(\w+),\s*(.+?),\s*([\w:]+)::(\w+)\(\.\.\.\)\)/',
            function (array $m): string {
                return sprintf(
                    'writeOptional(%s, %s, [](encoding::ByteBufferWriter &writer, const auto &value) { %s::%s(writer, value); })',
                    $m[1],
                    $m[2],
                    $this->mapClassName($m[3]),
                    $m[4]
                );
            },
            $e
        ) ?? $e;

        // Anything else using `(...)` needs a human: flag it rather than emit broken C++.
        if (str_contains($e, '(...)')) {
            $this->todos[] = $e . '  (PHP first-class callable syntax)';
        }

        return $e;
    }

    /**
     * Maps a class name used as a constructor.
     *
     * CacheableNbt is generic in C++ but not in PHP, so it needs its template argument spelled out;
     * the protocol only ever caches compound tags.
     */
    private function mapConstructorName(string $short): string
    {
        if ($short === 'CacheableNbt') {
            return 'types::CacheableNbt<nbt::tag::CompoundTag>';
        }
        return $this->mapClassName($short);
    }

    /** Maps a PHP short class name to its C++ spelling. */
    private function mapClassName(string $short): string
    {
        if (in_array($short, ['Byte', 'LE', 'BE', 'VarInt'], true)) {
            return 'encoding::' . $short;
        }
        if ($short === 'CommonTypes' || $short === 'BitSet' || $short === 'PacketBatch') {
            return 'serializer::' . $short;
        }
        if ($short === $this->className) {
            return $short;
        }
        if (isset(TypeMap::CLASS_TO_CPP[$short])) {
            return TypeMap::CLASS_TO_CPP[$short];
        }
        $fqn = $this->uses[$short] ?? null;
        if ($fqn !== null) {
            $mapped = TypeMap::typesClassToCpp($fqn);
            if ($mapped !== null) {
                return $mapped;
            }
        }
        return $short;
    }

    /** Returns the C++ type produced by a PHP reader call, or null if it is not a known reader. */
    private function inferReaderType(string $call): ?string
    {
        if (preg_match('/(\w+)::(\w+)\(/', $call, $m)) {
            return TypeMap::READER_TO_CPP[$m[1] . '::' . $m[2]] ?? null;
        }
        return null;
    }
}
