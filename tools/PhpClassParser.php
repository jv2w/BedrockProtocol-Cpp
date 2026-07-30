<?php

/**
 * Minimal structural parser for the BedrockProtocol PHP sources.
 *
 * The upstream sources are machine-generated and rigorously formatted (tabs, K&R braces, one
 * declaration per line), so a line-oriented parser with brace matching is reliable here and far
 * easier to audit than a full PHP AST walk.
 */

declare(strict_types=1);

namespace BedrockProtocolPort;

final class PhpMember
{
    public function __construct(
        public string $visibility,
        public string $name,
        public string $phpType,
        public ?string $default,
        public ?string $docblock,
        public ?string $trailingComment,
    ) {
    }
}

final class PhpConst
{
    public function __construct(
        public string $visibility,
        public string $name,
        public string $value,
        public ?string $docblock,
        public ?string $trailingComment,
    ) {
    }
}

final class PhpClass
{
    /** @var PhpConst[] */
    public array $consts = [];
    /** @var PhpMember[] */
    public array $properties = [];
    /** @var array<string, string> short class name => fully-qualified name */
    public array $uses = [];
    /** @var array<string, string> method name => raw body (without the enclosing braces) */
    public array $methodBodies = [];
    /** @var array<string, string> method name => raw signature line(s) */
    public array $methodSignatures = [];
    /** @var array<string, string|null> method name => preceding docblock */
    public array $methodDocblocks = [];
    /** @var string[] */
    public array $implements = [];

    public function __construct(
        public string $name,
        public ?string $extends,
        public ?string $classDocblock,
        public bool $isFinal,
    ) {
    }

    public function hasMethod(string $name): bool
    {
        return isset($this->methodBodies[$name]);
    }
}

final class PhpClassParser
{
    public static function parse(string $source): ?PhpClass
    {
        $uses = [];
        if (preg_match_all('/^use ([^;]+);$/m', $source, $m)) {
            foreach ($m[1] as $fqn) {
                $fqn = trim($fqn);
                if (str_starts_with($fqn, 'function ') || str_starts_with($fqn, 'const ')) {
                    continue;
                }
                // `use A\B\C as D;` binds the alias, not the last segment.
                $alias = null;
                if (preg_match('/^(.+?)\s+as\s+(\w+)$/i', $fqn, $aliasMatch)) {
                    $fqn = trim($aliasMatch[1]);
                    $alias = $aliasMatch[2];
                }
                $short = $alias ?? substr($fqn, strrpos($fqn, '\\') === false ? 0 : strrpos($fqn, '\\') + 1);
                $uses[$short] = $fqn;
            }
        }

        // Class declaration. The docblock is looked up separately - see docblockBefore().
        $pattern = '/^(final |abstract )?class (\w+)(?: extends (\w+))?(?: implements ([^{]+))?\{/m';
        if (!preg_match($pattern, $source, $m, PREG_OFFSET_CAPTURE)) {
            return null;
        }

        $class = new PhpClass(
            $m[2][0],
            ($m[3][0] ?? '') !== '' ? $m[3][0] : null,
            self::docblockBefore($source, $m[0][1]),
            trim($m[1][0] ?? '') === 'final',
        );
        if (($m[4][0] ?? '') !== '') {
            foreach (explode(',', $m[4][0]) as $iface) {
                $class->implements[] = trim($iface);
            }
        }
        $class->uses = $uses;

        $bodyStart = $m[0][1] + strlen($m[0][0]);
        $body = self::extractBraceBody($source, $bodyStart - 1);
        if ($body === null) {
            return null;
        }

        self::parseMembers($class, $body);
        self::parseMethods($class, $body);

        return $class;
    }

    /**
     * Returns the text between the brace at $openBracePos and its match, exclusive.
     *
     * String literals and comments are skipped so that braces inside them do not confuse the count.
     */
    public static function extractBraceBody(string $source, int $openBracePos): ?string
    {
        $depth = 0;
        $length = strlen($source);
        for ($i = $openBracePos; $i < $length; ++$i) {
            $c = $source[$i];
            if ($c === '\'' || $c === '"') {
                $quote = $c;
                for (++$i; $i < $length; ++$i) {
                    if ($source[$i] === '\\') {
                        ++$i;
                        continue;
                    }
                    if ($source[$i] === $quote) {
                        break;
                    }
                }
                continue;
            }
            if ($c === '/' && $i + 1 < $length && $source[$i + 1] === '/') {
                $i = strpos($source, "\n", $i) ?: $length;
                continue;
            }
            if ($c === '/' && $i + 1 < $length && $source[$i + 1] === '*') {
                $end = strpos($source, '*/', $i);
                $i = $end === false ? $length : $end + 1;
                continue;
            }
            if ($c === '#') {
                $i = strpos($source, "\n", $i) ?: $length;
                continue;
            }
            if ($c === '{') {
                ++$depth;
            } elseif ($c === '}') {
                --$depth;
                if ($depth === 0) {
                    return substr($source, $openBracePos + 1, $i - $openBracePos - 1);
                }
            }
        }
        return null;
    }

    private static function parseMembers(PhpClass $class, string $body): void
    {
        $lines = explode("\n", $body);
        $pendingDoc = null;
        $inDoc = false;
        $docBuffer = '';

        foreach ($lines as $line) {
            $trimmed = trim($line);

            if ($inDoc) {
                $docBuffer .= "\n" . $line;
                if (str_contains($trimmed, '*/')) {
                    $inDoc = false;
                    $pendingDoc = $docBuffer;
                }
                continue;
            }
            if (str_starts_with($trimmed, '/**')) {
                if (str_contains($trimmed, '*/')) {
                    $pendingDoc = $line;
                } else {
                    $inDoc = true;
                    $docBuffer = $line;
                }
                continue;
            }

            // Only consider members at class-body indentation (one tab); anything deeper is inside a method.
            if ($line !== '' && !preg_match('/^\t(?!\t)/', $line)) {
                if ($trimmed !== '') {
                    $pendingDoc = null;
                }
                continue;
            }

            if (preg_match('/^(public|private|protected) const (\w+)\s*=\s*(.+?);(?:\s*(\/\/.*))?$/', $trimmed, $m)) {
                $class->consts[] = new PhpConst($m[1], $m[2], trim($m[3]), $pendingDoc, $m[4] ?? null);
                $pendingDoc = null;
                continue;
            }

            if (preg_match('/^(public|private|protected)\s+(\??[\w|\\\\]+)\s+\$(\w+)\s*(?:=\s*(.+?))?;(?:\s*(\/\/.*))?$/', $trimmed, $m)) {
                $class->properties[] = new PhpMember($m[1], $m[3], $m[2], ($m[4] ?? '') !== '' ? trim($m[4]) : null, $pendingDoc, $m[5] ?? null);
                $pendingDoc = null;
                continue;
            }

            if ($trimmed !== '') {
                $pendingDoc = null;
            }
        }
    }

    /**
     * Returns the docblock immediately preceding $position, or null if there is none.
     *
     * This deliberately does NOT capture the docblock as part of the declaration regex: a lazy
     * `/\*\*.*?\*\//` group backtracks across intervening declarations when the nearest docblock is
     * not the one attached to the match, silently swallowing whole blocks of source.
     */
    private static function docblockBefore(string $body, int $position): ?string
    {
        $preceding = substr($body, 0, $position);
        // Only whitespace may separate the docblock from the declaration.
        if (!preg_match('/\*\/\s*$/', $preceding)) {
            return null;
        }
        $end = strrpos($preceding, '*/');
        $start = strrpos(substr($preceding, 0, $end), '/**');
        if ($start === false) {
            return null;
        }
        return substr($preceding, $start, $end - $start + 2);
    }

    private static function parseMethods(PhpClass $class, string $body): void
    {
        $offset = 0;
        $pattern = '/^\t(?:(?:public|private|protected)\s+)?(?:static\s+)?function (\w+)\s*\(/m';
        while (preg_match($pattern, $body, $m, PREG_OFFSET_CAPTURE, $offset)) {
            $name = $m[1][0];
            $docblock = self::docblockBefore($body, $m[0][1]);
            $parenPos = $m[0][1] + strlen($m[0][0]) - 1;

            // Walk to the opening brace of the body, skipping the parameter list.
            $depth = 0;
            $length = strlen($body);
            $bracePos = null;
            for ($i = $parenPos; $i < $length; ++$i) {
                if ($body[$i] === '(') {
                    ++$depth;
                } elseif ($body[$i] === ')') {
                    --$depth;
                    if ($depth === 0) {
                        $bracePos = strpos($body, '{', $i);
                        break;
                    }
                }
            }
            if ($bracePos === null) {
                break;
            }

            $signature = trim(substr($body, $m[0][1], $bracePos - $m[0][1]));

            $methodBody = self::extractBraceBody($body, $bracePos);
            $class->methodBodies[$name] = $methodBody ?? '';
            $class->methodSignatures[$name] = $signature;
            $class->methodDocblocks[$name] = $docblock;

            $offset = $bracePos + strlen($methodBody ?? '') + 1;
        }
    }
}
