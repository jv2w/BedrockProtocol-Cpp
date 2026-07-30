<?php

/**
 * Generates the mechanical parts of the C++ port from the PHP sources.
 *
 * Emitted artefacts:
 *   include/bedrock_protocol/protocol/ProtocolInfo.h              (fully generated, never hand-edited)
 *   include/bedrock_protocol/protocol/PacketHandlerInterface.h    (fully generated)
 *   include/bedrock_protocol/protocol/AllPackets.h                (fully generated)
 *   include/bedrock_protocol/protocol/PacketPool.registrations.inc(fully generated)
 *   include/bedrock_protocol/protocol/<Name>Packet.h              (skeleton, hand-finished)
 *   src/protocol/<Name>Packet.cpp                                 (skeleton + transpiled payloads)
 *   tools/manifest.json                                           (porting progress data)
 *
 * Usage: php tools/gen_from_php.php [--only=TextPacket,MovePlayerPacket] [--no-packets]
 */

declare(strict_types=1);

namespace BedrockProtocolPort;

require __DIR__ . '/TypeMap.php';
require __DIR__ . '/PhpClassParser.php';
require __DIR__ . '/Transpiler.php';

/**
 * Packets whose generated form did not compile and were completed by hand.
 *
 * Regenerating these would silently revert that work, so the generator skips them. Remove a name
 * from this list only if you also intend to redo the manual translation.
 */
const HAND_FINISHED_PACKETS = [
    'CameraInstructionPacket', 'TextPacket', 'ClientMovementPredictionSyncPacket', 'StartGamePacket',
    'SetHudPacket', 'AddItemActorPacket', 'SetScoreboardIdentityPacket', 'EduUriResourcePacket',
    'ResourcePackClientResponsePacket', 'VoxelShapesPacket', 'RequestAbilityPacket',
    'ClientboundMapItemDataPacket', 'FeatureRegistryPacket', 'ServerPresenceInfoPacket', 'AddActorPacket',
    'ClientboundControlSchemeSetPacket', 'ShowStoreOfferPacket', 'EditorNetworkPacket',
    'ResourcePacksInfoPacket', 'UpdateTradePacket', 'CraftingDataPacket', 'ServerboundPackSettingChangePacket',
    'PlayerListPacket', 'ServerStoreInfoPacket', 'PlayerLocationPacket', 'ClientCameraAimAssistPacket',
    'GameRulesChangedPacket', 'InventoryContentPacket', 'ServerboundDataStorePacket', 'PlayerAuthInputPacket',
    'ClientboundAttributeLayerSyncPacket', 'UpdateClientOptionsPacket', 'ClientboundDebugRendererPacket',
    'SpawnParticleEffectPacket', 'GraphicsOverrideParameterPacket', 'LoginPacket', 'JigsawStructureDataPacket',
    'SetActorLinkPacket', 'LevelEventGenericPacket', 'EducationSettingsPacket', 'MoveActorDeltaPacket',
    'DisconnectPacket', 'AvailableCommandsPacket', 'MovementEffectPacket', 'MobArmorEquipmentPacket',
    'SubChunkRequestPacket', 'SubChunkPacket', 'SyncWorldClocksPacket', 'SetPlayerInventoryOptionsPacket',
    'SetScorePacket', 'PlayerVideoCapturePacket', 'ResourcePackStackPacket', 'AddVolumeEntityPacket',
    'MobEquipmentPacket', 'SyncActorPropertyPacket', 'InventoryTransactionPacket',
    'NetworkChunkPublisherUpdatePacket', 'DimensionDataPacket', 'PlayerUpdateEntityOverridesPacket',
    'StructureTemplateDataResponsePacket', 'ServerboundLoadingScreenPacket', 'AvailableActorIdentifiersPacket',
    'PlayerSkinPacket', 'UpdateAbilitiesPacket', 'LevelChunkPacket', 'ClientboundDataStorePacket',
    'InventorySlotPacket', 'CameraAimAssistPacket', 'AddPlayerPacket', 'SetActorDataPacket',
    'BlockActorDataPacket', 'PositionTrackingDBServerBroadcastPacket', 'UpdateEquipPacket',
];

/**
 * ProtocolInfo constants renamed to avoid a macro of the same name.
 *
 * Endstone's detail.h defines MINECRAFT_VERSION, which would be substituted away wherever the
 * constant appears. Renaming beats #undef, which leaks into every header included afterwards.
 */
const PROTOCOL_INFO_RENAMES = [
    'MINECRAFT_VERSION' => 'MINECRAFT_VERSION_DISPLAY',
];

const PHP_SRC = 'c:/Users/User/Desktop/BetterPMMP/source/vendor/pocketmine/bedrock-protocol/src';
const CPP_ROOT = __DIR__ . '/..';
const INCLUDE_DIR = CPP_ROOT . '/include/bedrock_protocol/protocol';
const SRC_DIR = CPP_ROOT . '/src/protocol';

$options = getopt('', ['only::', 'no-packets', 'protect::']);
$only = isset($options['only']) ? explode(',', (string)$options['only']) : null;

/**
 * Packets that must never be overwritten by the generator.
 *
 * Anything the generator cannot express - PHP helper methods, hand-resolved match expressions - is
 * finished by hand and would be silently reverted by the next run. Extend this list (or pass
 * --protect=A,B) whenever a packet is completed manually.
 */
$protected = array_flip(array_merge(
    // PHP declares getCommandMessage/putCommandMessage helpers the generator cannot express.
    ['CommandOutputPacket',
     // fromDefinitions() interns strings through a closure; hand-ported.
     'BiomeDefinitionListPacket'],
    // Packets finished by hand because the transpiler could not translate them: match expressions,
    // string interpolation, polymorphic containers, non-default-constructible members.
    HAND_FINISHED_PACKETS,
    isset($options['protect']) ? explode(',', (string)$options['protect']) : []
));

@mkdir(INCLUDE_DIR, 0777, true);
@mkdir(SRC_DIR, 0777, true);

echo "Generating ProtocolInfo.h...\n";
generateProtocolInfo();

$packets = [];
foreach (glob(PHP_SRC . '/*Packet.php') as $file) {
    $name = basename($file, '.php');
    if (in_array($name, ['Packet', 'DataPacket', 'ClientboundPacket', 'ServerboundPacket'], true)) {
        continue;
    }
    $source = file_get_contents($file);
    $class = PhpClassParser::parse($source);
    // A handful of packets extend another packet rather than DataPacket directly.
    if ($class === null || $class->extends === null || !str_ends_with($class->extends, 'Packet')) {
        echo "  skipped $name (not a DataPacket subclass)\n";
        continue;
    }
    $packets[$name] = $class;
}
ksort($packets);

// Packets that others derive from cannot be marked final.
$basePackets = [];
foreach ($packets as $class) {
    if ($class->extends !== 'DataPacket') {
        $basePackets[$class->extends] = true;
    }
}
echo 'Parsed ' . count($packets) . " packet classes.\n";

echo "Generating PacketHandlerInterface.h, AllPackets.h and the pool registrations...\n";
generateHandlerInterface($packets);
generateAllPacketsHeader($packets);
generatePoolRegistrations($packets);

$manifest = [];
if (!isset($options['no-packets'])) {
    foreach ($packets as $name => $class) {
        if ($only !== null && !in_array($name, $only, true)) {
            continue;
        }
        if (isset($protected[$name])) {
            echo "  protected, not regenerated: $name\n";
            continue;
        }
        $manifest[$name] = generatePacket($name, $class, isset($basePackets[$name]));
    }
    file_put_contents(__DIR__ . '/manifest.json', json_encode($manifest, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES));

    $totalTodos = array_sum(array_column($manifest, 'todoCount'));
    $clean = count(array_filter($manifest, static fn(array $e): bool => $e['todoCount'] === 0));
    echo sprintf("Generated %d packets: %d fully transpiled, %d PORT-TODO markers remaining.\n", count($manifest), $clean, $totalTodos);
}

// ---------------------------------------------------------------------------

function licenseHeader(string $phpOrigin): string
{
    return <<<HEADER
/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: {$phpOrigin}
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

HEADER;
}

/** Converts a PHP docblock to a Doxygen block at the given indentation, per PORTING.md §4. */
function convertDocblock(?string $doc, string $indent): string
{
    if ($doc === null) {
        return '';
    }
    // A one-line docblock needs no reflowing.
    $collapsed = trim($doc);
    if (!str_contains($collapsed, "\n")) {
        return $indent . $collapsed . "\n";
    }
    $lines = [];
    foreach (explode("\n", $doc) as $line) {
        $line = trim($line);
        if ($line === '') {
            continue;
        }
        // Drop analyser-only annotations.
        if (preg_match('/^\*\s*@(phpstan-|psalm-|var\b|param\s+\S+\s+\$)/', $line)) {
            if (preg_match('/^\*\s*@param\s+\S+\s+\$(\w+)\s+(.+)$/', $line, $m)) {
                $lines[] = $indent . ' * @param ' . $m[1] . ' ' . $m[2];
            }
            continue;
        }
        if (preg_match('/^\*\s*@return\s+\S+\s*$/', $line)) {
            continue;
        }
        if (preg_match('/^\*\s*@return\s+\S+\s+(.+)$/', $line, $m)) {
            $lines[] = $indent . ' * @return ' . $m[1];
            continue;
        }
        $line = str_replace(['{@link ', '}'], ['@see ', ''], $line);
        $lines[] = $indent . ($line === '/**' ? '/**' : ' ' . $line);
    }
    if ($lines === []) {
        return '';
    }
    // Re-add the opening if the loop dropped it.
    if (!str_contains($lines[0], '/**')) {
        array_unshift($lines, $indent . '/**');
    }
    if (!str_contains(end($lines), '*/')) {
        $lines[] = $indent . ' */';
    }
    return implode("\n", $lines) . "\n";
}

function generateProtocolInfo(): void
{
    $source = file_get_contents(PHP_SRC . '/ProtocolInfo.php');
    $class = PhpClassParser::parse($source);
    if ($class === null) {
        throw new \RuntimeException('Could not parse ProtocolInfo.php');
    }

    $body = '';
    foreach ($class->consts as $const) {
        $body .= convertDocblock($const->docblock, '    ');
        $value = $const->value;
        // Renamed to dodge a macro of the same name; see the class docblock below.
        $name = PROTOCOL_INFO_RENAMES[$const->name] ?? $const->name;
        if (isStringLiteral($value)) {
            $body .= sprintf('    static constexpr std::string_view %s = %s;', $name, toCppStringLiteral($value));
        } else {
            $type = str_ends_with($const->name, '_PACKET') ? 'std::uint32_t' : 'std::int32_t';
            $body .= sprintf('    static constexpr %s %s = %s;', $type, $name, $value);
        }
        $body .= ($const->trailingComment !== null ? '  ' . $const->trailingComment : '') . "\n";
    }

    $contents = licenseHeader('src/ProtocolInfo.php') . <<<CPP

#pragma once

#include <cstdint>
#include <string_view>

namespace bedrock_protocol {

/**
 * Version numbers and packet IDs for the current Minecraft PE protocol
 *
 * @note One deviation from PHP: the constant PHP calls MINECRAFT_VERSION is MINECRAFT_VERSION_DISPLAY
 *       here. Endstone's detail.h defines MINECRAFT_VERSION as an object-like macro, so the original
 *       name would be substituted away at every use site. Renaming is preferable to #undef, which
 *       would silently leak into every header included afterwards.
 *
 * This file is generated automatically by tools/gen_from_php.php, do not edit it manually.
 */
struct ProtocolInfo final {
    ProtocolInfo() = delete;

{$body}};

}  // namespace bedrock_protocol

CPP;
    file_put_contents(INCLUDE_DIR . '/ProtocolInfo.h', $contents);
}

/** Derives the handler method name from a packet's handle() body, e.g. handleMovePlayer. */
function handlerMethodName(PhpClass $class): ?string
{
    $body = $class->methodBodies['handle'] ?? '';
    if (preg_match('/\$handler->(\w+)\(/', $body, $m)) {
        return $m[1];
    }
    return null;
}

/** @param array<string, PhpClass> $packets */
function generateHandlerInterface(array $packets): void
{
    $forward = '';
    $methods = '';
    foreach ($packets as $name => $class) {
        $handler = handlerMethodName($class);
        if ($handler === null) {
            continue;
        }
        $forward .= "class {$name};\n";
        $methods .= "    virtual bool {$handler}({$name} &packet) { return false; }\n";
    }

    $contents = licenseHeader('src/PacketHandlerInterface.php + src/PacketHandlerDefaultImplTrait.php') . <<<CPP

#pragma once

namespace bedrock_protocol {

{$forward}
/**
 * Receives decoded packets dispatched by Packet::handle().
 *
 * Every method defaults to returning false ("not handled"), mirroring PacketHandlerDefaultImplTrait
 * in the PHP original, so implementations only override the packets they care about.
 *
 * This file is generated automatically by tools/gen_from_php.php, do not edit it manually.
 */
class PacketHandlerInterface {
public:
    virtual ~PacketHandlerInterface() = default;

{$methods}};

}  // namespace bedrock_protocol

CPP;
    file_put_contents(INCLUDE_DIR . '/PacketHandlerInterface.h', $contents);
}

/** @param array<string, PhpClass> $packets */
function generateAllPacketsHeader(array $packets): void
{
    $includes = '';
    foreach (array_keys($packets) as $name) {
        $includes .= "#include \"bedrock_protocol/protocol/{$name}.h\"\n";
    }

    $contents = licenseHeader('src/PacketPool.php (include list)') . <<<CPP

#pragma once

/**
 * Convenience header pulling in every packet class.
 *
 * This file is generated automatically by tools/gen_from_php.php, do not edit it manually.
 */

{$includes}
CPP;
    file_put_contents(INCLUDE_DIR . '/AllPackets.h', $contents);
}

/** @param array<string, PhpClass> $packets */
function generatePoolRegistrations(array $packets): void
{
    $lines = '';
    foreach (array_keys($packets) as $name) {
        $lines .= "registerPacket<{$name}>();\n";
    }
    file_put_contents(INCLUDE_DIR . '/PacketPool.registrations.inc', $lines);
}

/**
 * Works out the C++ type of every property.
 *
 * PHP's `int` carries no width, so integer fields are typed from the wire function that fills them
 * in decodePayload(); everything else comes from the declared type. See PORTING.md §3.1.
 *
 * @return array<string, string> property name => C++ type
 */
/** Returns whether a PHP literal is a quoted string, in either quoting style. */
function isStringLiteral(string $value): bool
{
    return (bool)preg_match('/^([\'"]).*\1$/s', trim($value));
}

/** Converts a PHP string literal to a C++ one, preserving the contents. */
function toCppStringLiteral(string $value): string
{
    return '"' . substr(trim($value), 1, -1) . '"';
}

/** Returns a class constant by name, or null if the class does not declare it. */
function findConst(PhpClass $class, string $name): ?PhpConst
{
    foreach ($class->consts as $const) {
        if ($const->name === $name) {
            return $const;
        }
    }
    return null;
}

/**
 * Chooses the C++ type for a class constant.
 *
 * PHP constants are untyped, but they are almost always compared against a field. Adopting that
 * field's type keeps `field == CONST` free of signed/unsigned mismatch warnings.
 */
function constType(PhpClass $class, PhpConst $const, array $fieldTypes): string
{
    if (isStringLiteral($const->value)) {
        return 'std::string_view';
    }
    $bodies = ($class->methodBodies['decodePayload'] ?? '') . ($class->methodBodies['encodePayload'] ?? '');
    $pattern = '/\$this->(\w+)\s*(?:===|!==|==|!=)\s*(?:self::|static::|' . preg_quote($class->name, '/') . '::)?' . preg_quote($const->name, '/') . '\b/';
    if (preg_match($pattern, $bodies, $m) && isset($fieldTypes[$m[1]])) {
        $type = $fieldTypes[$m[1]];
        if (str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint')) {
            return $type;
        }
    }
    // Reverse order: `CONST === $this->field`
    $pattern = '/(?:self::|static::)?' . preg_quote($const->name, '/') . '\s*(?:===|!==|==|!=)\s*\$this->(\w+)/';
    if (preg_match($pattern, $bodies, $m) && isset($fieldTypes[$m[1]])) {
        $type = $fieldTypes[$m[1]];
        if (str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint')) {
            return $type;
        }
    }
    return 'std::int32_t';
}

function inferFieldTypes(PhpClass $class): array
{
    $decode = $class->methodBodies['decodePayload'] ?? '';
    $readerByField = [];
    if (preg_match_all('/\$this->(\w+)(\[\])? = ([\w\\\\]+)::(\w+)\(/', $decode, $m, PREG_SET_ORDER)) {
        foreach ($m as $match) {
            $readerByField[$match[1]] = ['call' => $match[3] . '::' . $match[4], 'isElement' => $match[2] === '[]'];
        }
    }
    // readOptional's own return type says nothing; the width comes from the callable passed to it.
    if (preg_match_all('/\$this->(\w+) = [\w\\\\]+::readOptional\(\$in,\s*([\w\\\\]+)::(\w+)\(\.\.\.\)\)/', $decode, $m, PREG_SET_ORDER)) {
        foreach ($m as $match) {
            $readerByField[$match[1]] = ['call' => $match[2] . '::' . $match[3], 'isElement' => false];
        }
    }

    $types = [];
    foreach ($class->properties as $property) {
        $types[$property->name] = resolvePropertyType($class, $property, $readerByField[$property->name] ?? null);
    }
    return $types;
}

function resolvePropertyType(PhpClass $class, PhpMember $property, ?array $reader): string
{
    $phpType = $property->phpType;
    $nullable = str_starts_with($phpType, '?');
    $bare = ltrim($phpType, '?');

    $inner = resolveBareType($class, $bare, $property, $reader);
    return $nullable ? "std::optional<{$inner}>" : $inner;
}

function resolveBareType(PhpClass $class, string $bare, PhpMember $property, ?array $reader): string
{
    if (isset(TypeMap::SCALAR_TO_CPP[$bare])) {
        return TypeMap::SCALAR_TO_CPP[$bare];
    }

    if ($bare === 'int') {
        if ($reader !== null && isset(TypeMap::READER_TO_CPP[$reader['call']])) {
            return TypeMap::READER_TO_CPP[$reader['call']];
        }
        // No wire evidence: fall back to the widest signed type and flag it for review.
        return 'std::int32_t /* PORT-TODO: width not inferable from decodePayload */';
    }

    if ($bare === 'array') {
        $element = arrayElementType($class, $property, $reader);
        return "std::vector<{$element}>";
    }

    // CacheableNbt is generic over its root tag type, which PHP records only in the docblock.
    if ($bare === 'CacheableNbt') {
        $tag = 'nbt::tag::CompoundTag';
        if ($property->docblock !== null && preg_match('/CacheableNbt<\\\\?([\w\\\\]+)>/', $property->docblock, $m)) {
            $short = substr($m[1], strrpos($m[1], '\\') === false ? 0 : strrpos($m[1], '\\') + 1);
            $tag = 'nbt::tag::' . $short;
        }
        return "types::CacheableNbt<{$tag}>";
    }

    return mapDeclaredClass($class, $bare);
}

function arrayElementType(PhpClass $class, PhpMember $property, ?array $reader): string
{
    // Prefer the @var/@phpstan-var annotation, which names the element type explicitly.
    if ($property->docblock !== null) {
        if (preg_match('/@(?:phpstan-)?var\s+(?:list<|array<[^,]+,\s*)?([\w\\\\]+)(?:\[\]|>)/', $property->docblock, $m)) {
            $short = $m[1];
            if (isset(TypeMap::SCALAR_TO_CPP[$short])) {
                return TypeMap::SCALAR_TO_CPP[$short];
            }
            if ($short === 'int') {
                return $reader !== null ? (TypeMap::READER_TO_CPP[$reader['call']] ?? 'std::int32_t') : 'std::int32_t';
            }
            return mapDeclaredClass($class, $short);
        }
    }
    if ($reader !== null && isset(TypeMap::READER_TO_CPP[$reader['call']])) {
        return TypeMap::READER_TO_CPP[$reader['call']];
    }
    return 'PORT_TODO_ELEMENT_TYPE';
}

function mapDeclaredClass(PhpClass $class, string $short): string
{
    if (isset(TypeMap::CLASS_TO_CPP[$short])) {
        return TypeMap::CLASS_TO_CPP[$short];
    }
    $fqn = $class->uses[$short] ?? null;
    if ($fqn !== null) {
        $mapped = TypeMap::typesClassToCpp($fqn);
        if ($mapped !== null) {
            return $mapped;
        }
    }
    return $short;
}

/**
 * Rewrites the class names inside a constant's value expression.
 *
 * PHP constants often forward to another class (`ARG_TYPE_INT = ArgTypes::INT`), which only resolves
 * in C++ once the name is namespace-qualified.
 */
function mapConstValue(PhpClass $class, string $value): string
{
    $value = preg_replace('/\b(?:self|static)::/', '', $value) ?? $value;
    return preg_replace_callback('/(?<![\w:])([A-Z]\w*)::/', static function (array $m) use ($class): string {
        return mapDeclaredClass($class, $m[1]) . '::';
    }, $value) ?? $value;
}

/** Collects the include paths a packet needs, based on the C++ types it mentions. */
function collectIncludes(array $fieldTypes, PhpClass $class): array
{
    $includes = [];
    $constValues = '';
    foreach ($class->consts as $const) {
        $constValues .= ' ' . mapConstValue($class, $const->value);
    }
    // Field initialisers can name a class nothing else in the packet mentions.
    foreach ($class->properties as $property) {
        if ($property->default !== null) {
            $constValues .= ' ' . mapConstValue($class, $property->default);
        }
    }
    // Factory bodies can name a type that neither the fields nor the payload methods mention.
    $factoryBodies = '';
    foreach (array_keys(factoryMethods($class)) as $factory) {
        $factoryBodies .= ' ' . ($class->methodBodies[$factory] ?? '');
    }

    $blob = implode(' ', $fieldTypes) . ' ' . ($class->methodBodies['decodePayload'] ?? '') . ' '
        . ($class->methodBodies['encodePayload'] ?? '') . $constValues . $factoryBodies;

    foreach (TypeMap::CPP_TYPE_TO_INCLUDE as $type => $path) {
        if (str_contains($blob, $type)) {
            $includes[$path] = true;
        }
    }
    foreach ($class->uses as $short => $fqn) {
        $mapped = TypeMap::typesClassToCpp($fqn);
        if ($mapped !== null && (str_contains($blob, $mapped) || str_contains($blob, $short))) {
            $includes[TypeMap::typesCppToInclude($mapped)] = true;
            continue;
        }
    }

    // A packet can embed another packet as a field (AddPlayerPacket holds an UpdateAbilitiesPacket).
    // Packets share one PHP namespace, so these references carry no `use` statement to key off.
    if (preg_match_all('/\b([A-Z]\w*Packet)\b/', $blob, $packetRefs)) {
        $selfAndBases = ['DataPacket' => true, 'ClientboundPacket' => true, 'ServerboundPacket' => true,
                         $class->name => true];
        foreach (array_unique($packetRefs[1]) as $referenced) {
            if (!isset($selfAndBases[$referenced])) {
                $includes["bedrock_protocol/protocol/{$referenced}.h"] = true;
            }
        }
    }
    return array_keys($includes);
}

function generatePacket(string $name, PhpClass $class, bool $isBaseOfAnotherPacket): array
{
    $fieldTypes = inferFieldTypes($class);
    $transpiler = new Transpiler($class->uses, $fieldTypes, $name, $class->extends);

    $decode = $transpiler->transpile($class->methodBodies['decodePayload'] ?? '', true);
    $encode = $transpiler->transpile($class->methodBodies['encodePayload'] ?? '', false);
    $todos = array_merge($decode->todos, $encode->todos);

    writePacketHeader($name, $class, $fieldTypes, $isBaseOfAnotherPacket);
    writePacketSource($name, $class, $fieldTypes, $decode, $encode);

    return [
        'name' => $name,
        'networkId' => $class->consts[0]->value ?? null,
        'implements' => $class->implements,
        'fieldCount' => count($class->properties),
        'todoCount' => count($todos),
        'todos' => $todos,
    ];
}

function writePacketHeader(string $name, PhpClass $class, array $fieldTypes, bool $isBaseOfAnotherPacket): void
{
    $bases = ['public ' . $class->extends];
    foreach ($class->implements as $iface) {
        if ($iface === 'ClientboundPacket' || $iface === 'ServerboundPacket') {
            $bases[] = 'public ' . $iface;
        }
    }

    $includes = collectIncludes($fieldTypes, $class);
    if ($class->extends !== 'DataPacket') {
        $includes[] = "bedrock_protocol/protocol/{$class->extends}.h";
    }
    $includeBlock = '';
    foreach ($includes as $path) {
        $includeBlock .= "#include \"{$path}\"\n";
    }

    // Constants, keeping PHP's ordering and visibility.
    $publicConsts = '';
    $privateConsts = '';
    foreach ($class->consts as $const) {
        if ($const->name === 'NETWORK_ID') {
            continue;
        }
        $type = constType($class, $const, $fieldTypes);
        $value = $type === 'std::string_view' ? toCppStringLiteral($const->value) : mapConstValue($class, $const->value);
        $line = convertDocblock($const->docblock, '    ')
            . sprintf('    static constexpr %s %s = %s;', $type, $const->name, $value)
            . ($const->trailingComment !== null ? '  ' . $const->trailingComment : '') . "\n";
        if ($const->visibility === 'public') {
            $publicConsts .= $line;
        } else {
            $privateConsts .= $line;
        }
    }

    $fields = '';
    foreach ($class->properties as $property) {
        $type = $fieldTypes[$property->name];
        $default = cppDefault($class, $type, $property->default);
        $fields .= convertDocblock($property->docblock, '    ')
            . sprintf('    %s %s%s;', $type, $property->name, $default !== null ? ' = ' . $default : '')
            . ($property->trailingComment !== null ? '  ' . $property->trailingComment : '') . "\n";
    }

    $createDecl = '';
    if ($class->hasMethod('create')) {
        $params = createParams($class, $fieldTypes, true);
        $createDecl = convertDocblock($class->methodDocblocks['create'] ?? null, '    ')
            . sprintf("    static %s create(%s);\n\n", $name, $params);
    }

    // PocketMine's convenience constructors, which plugin code uses far more than raw create().
    $transpiler = new Transpiler($class->uses, $fieldTypes, $name, $class->extends);
    $renames = factoryRenames($class, $fieldTypes);
    $privateFactoryDecls = '';
    foreach (factoryMethods($class) as $factory => $visibility) {
        if (transpileFactoryBody($class->methodBodies[$factory], $name, $fieldTypes, $transpiler, $renames) === null) {
            continue;  // shape not recognised; see transpileFactoryBody()
        }
        $renamed = $renames[$factory] ?? $factory;
        $note = isset($renames[$factory])
            ? "    /** @note PHP calls this {$factory}(); renamed because C++ cannot have a method and a field of the same name. */\n"
            : '';
        $decl = convertDocblock($class->methodDocblocks[$factory] ?? null, '    ')
            . $note
            . sprintf("    static %s %s(%s);\n\n", $name, $renamed,
                      factoryParams($class, $fieldTypes, $class->methodSignatures[$factory], true));
        if ($visibility === 'public') {
            $createDecl .= $decl;
        } else {
            $privateFactoryDecls .= $decl;
        }
    }

    $canBeSentBeforeLogin = '';
    if ($class->hasMethod('canBeSentBeforeLogin')) {
        $returns = str_contains($class->methodBodies['canBeSentBeforeLogin'], 'true') ? 'true' : 'false';
        $canBeSentBeforeLogin = "    [[nodiscard]] bool canBeSentBeforeLogin() const override { return {$returns}; }\n";
    }

    $classDoc = convertDocblock($class->classDocblock, '');
    $privateBody = $privateConsts . $privateFactoryDecls;
    $privateSection = $privateBody !== '' ? "\nprivate:\n{$privateBody}" : '';

    $basesStr = implode(', ', $bases);
    $finalKeyword = $isBaseOfAnotherPacket ? '' : ' final';
    $networkIdConst = findConst($class, 'NETWORK_ID');
    $networkIdValue = $networkIdConst !== null
        ? preg_replace('/^ProtocolInfo::/', '', $networkIdConst->value)
        : 'PORT_TODO_NETWORK_ID';
    $contents = licenseHeader("src/{$name}.php") . <<<CPP

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
{$includeBlock}
namespace bedrock_protocol {

class PacketHandlerInterface;

{$classDoc}class {$name}{$finalKeyword} : {$basesStr} {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::{$networkIdValue};

{$publicConsts}
{$fields}
{$createDecl}    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "{$name}"; }
{$canBeSentBeforeLogin}    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
{$privateSection}};

}  // namespace bedrock_protocol

CPP;

    file_put_contents(INCLUDE_DIR . "/{$name}.h", $contents);
}

/**
 * Returns the packet's static factory methods other than create().
 *
 * PocketMine gives most packets convenience constructors (SetTitlePacket::title(),
 * BossEventPacket::show(), MovePlayerPacket::simple(), ...) which plugin code uses far more than the
 * raw create(). They follow a narrow set of shapes, so they transpile mechanically.
 *
 * @return array<string, string> method name => visibility
 */
function factoryMethods(PhpClass $class): array
{
    $factories = [];
    foreach ($class->methodSignatures as $name => $signature) {
        if ($name === 'create' || !str_contains($signature, 'static function')) {
            continue;
        }
        if (!preg_match('/\)\s*:\s*self\s*$/', $signature)) {
            continue;  // only self-returning factories
        }
        $factories[$name] = str_starts_with(trim($signature), 'private') ? 'private' : 'public';
    }
    return $factories;
}

/**
 * Builds the rename map for factories whose name collides with a field.
 *
 * PHP lets a class have both a `$type` property and a `type()` method; C++ does not. Colliding
 * factories are prefixed with "make" - they are private helpers in the PHP original too, so nothing
 * user-facing changes.
 *
 * @return array<string, string> PHP factory name => C++ method name
 */
function factoryRenames(PhpClass $class, array $fieldTypes): array
{
    $renames = [];
    foreach (array_keys(factoryMethods($class)) as $factory) {
        if (isset($fieldTypes[$factory])) {
            $renames[$factory] = 'make' . ucfirst($factory);
        }
    }
    return $renames;
}

/**
 * Renders a factory's parameter list.
 *
 * A parameter almost always names the field it fills, so the field's inferred width applies;
 * otherwise the declared PHP type is mapped directly.
 */
function factoryParams(PhpClass $class, array $fieldTypes, string $signature, bool $withDefaults): string
{
    if (!preg_match('/\((.*)\)\s*:/s', $signature, $m)) {
        return '';
    }
    $params = [];
    foreach (splitParams($m[1]) as $param) {
        if (!preg_match('/^\s*(\??[\w|\\\\]+)?\s*\$(\w+)\s*(?:=\s*(.+))?$/s', trim($param), $pm)) {
            continue;
        }
        $paramName = $pm[2];
        $type = $fieldTypes[$paramName] ?? mapPhpParamType($class, $pm[1] ?? '');
        $rendered = "{$type} {$paramName}";
        if ($withDefaults && ($pm[3] ?? '') !== '') {
            $default = cppDefault($class, $type, trim($pm[3]));
            if ($default !== null) {
                $rendered .= ' = ' . $default;
            }
        }
        $params[] = $rendered;
    }
    return implode(', ', $params);
}

/** Splits a PHP parameter list on commas that are not nested inside brackets. */
function splitParams(string $list): array
{
    $params = [];
    $depth = 0;
    $current = '';
    for ($i = 0, $n = strlen($list); $i < $n; ++$i) {
        $c = $list[$i];
        if ($c === '(' || $c === '[' || $c === '<') {
            ++$depth;
        } elseif ($c === ')' || $c === ']' || $c === '>') {
            --$depth;
        }
        if ($c === ',' && $depth === 0) {
            $params[] = $current;
            $current = '';
            continue;
        }
        $current .= $c;
    }
    if (trim($current) !== '') {
        $params[] = $current;
    }
    return $params;
}

/** Maps a PHP parameter type declaration when no field of the same name pins it down. */
function mapPhpParamType(PhpClass $class, string $phpType): string
{
    $nullable = str_starts_with($phpType, '?');
    $bare = ltrim($phpType, '?');
    $mapped = match ($bare) {
        'int' => 'std::int32_t',
        'string' => 'std::string',
        'bool' => 'bool',
        'float' => 'float',
        '' => 'PORT_TODO_PARAM_TYPE',
        default => mapDeclaredClass($class, $bare),
    };
    return $nullable ? "std::optional<{$mapped}>" : $mapped;
}

/**
 * Transpiles a factory body.
 *
 * The shapes in the PHP sources are: `$result = new self;`, `$result = self::other(...);`,
 * `$result->field = $value;`, `return $result;` and `return self::other(...);`.
 */
function transpileFactoryBody(string $phpBody, string $className, array $fieldTypes, Transpiler $transpiler,
                              array $renames = []): ?string
{
    $callName = static fn(string $php): string => $renames[$php] ?? $php;
    $lines = [];
    foreach (explode("\n", trim($phpBody, "\n")) as $raw) {
        $line = trim($raw);
        if ($line === '') {
            continue;
        }
        if (str_starts_with($line, '//')) {
            $lines[] = '    ' . $line;
            continue;
        }
        if (preg_match('/^\$result = new self;$/', $line)) {
            $lines[] = "    {$className} result;";
        } elseif (preg_match('/^\$result = self::(\w+)\((.*)\);$/', $line, $m)) {
            $lines[] = sprintf('    auto result = %s(%s);', $callName($m[1]), $transpiler->translateExpression($m[2]));
        } elseif (preg_match('/^\$result->(\w+) = (.+);$/', $line, $m)) {
            $type = $fieldTypes[$m[1]] ?? '';
            $trivial = in_array($type, ['bool', 'float', 'double'], true)
                || str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint');
            $value = $transpiler->translateExpression($m[2]);
            // Only a bare parameter can be moved from; an expression may be used again below.
            $movable = !$trivial && preg_match('/^\w+$/', $value) === 1;
            $lines[] = sprintf('    result.%s = %s;', $m[1], $movable ? "std::move({$value})" : $value);
        } elseif ($line === 'return $result;') {
            $lines[] = '    return result;';
        } elseif (preg_match('/^return self::(\w+)\((.*)\);$/', $line, $m)) {
            $lines[] = sprintf('    return %s(%s);', $callName($m[1]), $transpiler->translateExpression($m[2]));
        } else {
            return null;  // an unrecognised shape: skip the whole factory rather than emit guesswork
        }
    }
    return implode("\n", $lines);
}

/** Renders create()'s parameter list, reusing each field's inferred C++ type. */
function createParams(PhpClass $class, array $fieldTypes, bool $declaration): string
{
    $signature = $class->methodSignatures['create'] ?? '';
    if (!preg_match('/\((.*)\)\s*:/s', $signature, $m)) {
        return '';
    }
    $params = [];
    foreach (explode(',', $m[1]) as $param) {
        $param = trim($param);
        if ($param === '') {
            continue;
        }
        if (!preg_match('/\$(\w+)/', $param, $pm)) {
            continue;
        }
        $paramName = $pm[1];
        $type = $fieldTypes[$paramName] ?? 'PORT_TODO_PARAM_TYPE';
        $byValue = in_array($type, ['bool', 'float', 'double'], true) || str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint');
        $params[] = $byValue ? "{$type} {$paramName}" : "{$type} {$paramName}";
    }
    return implode(', ', $params);
}

/** Chooses a C++ initialiser for a field, mirroring PHP's default where there is one. */
function cppDefault(PhpClass $class, string $type, ?string $phpDefault): ?string
{
    if ($phpDefault !== null) {
        return match (true) {
            $phpDefault === 'null' => str_starts_with($type, 'std::optional') ? 'std::nullopt' : 'nullptr',
            $phpDefault === '[]' => null,
            $phpDefault === 'true' || $phpDefault === 'false' => $phpDefault,
            isStringLiteral($phpDefault) => toCppStringLiteral($phpDefault),
            default => mapConstValue($class, $phpDefault),
        };
    }
    // PHP leaves typed properties uninitialised; C++ must value-initialise to avoid UB.
    if (str_starts_with($type, 'std::int') || str_starts_with($type, 'std::uint')) {
        return '0';
    }
    if ($type === 'float' || $type === 'double') {
        return '0.0F';
    }
    if ($type === 'bool') {
        return 'false';
    }
    return null;
}

function writePacketSource(string $name, PhpClass $class, array $fieldTypes, TranspileResult $decode, TranspileResult $encode): void
{
    $handler = handlerMethodName($class) ?? 'handleUnknown';

    $createImpl = '';
    if ($class->hasMethod('create')) {
        $params = createParams($class, $fieldTypes, false);
        $assignments = '';
        if (preg_match_all('/\$result->(\w+) = \$(\w+);/', $class->methodBodies['create'], $m, PREG_SET_ORDER)) {
            foreach ($m as $match) {
                $type = $fieldTypes[$match[1]] ?? '';
                $trivial = in_array($type, ['bool', 'float', 'double'], true)
                    || str_starts_with($type, 'std::int')
                    || str_starts_with($type, 'std::uint');
                $value = $trivial ? $match[2] : "std::move({$match[2]})";
                $assignments .= "    result.{$match[1]} = {$value};\n";
            }
        }
        $createImpl = <<<CPP
{$name} {$name}::create({$params})
{
    {$name} result;
{$assignments}    return result;
}


CPP;
    }

    $transpiler = new Transpiler($class->uses, $fieldTypes, $name, $class->extends);
    $renames = factoryRenames($class, $fieldTypes);
    foreach (factoryMethods($class) as $factory => $visibility) {
        $body = transpileFactoryBody($class->methodBodies[$factory], $name, $fieldTypes, $transpiler, $renames);
        if ($body === null) {
            continue;
        }
        $createImpl .= sprintf(
            "%s %s::%s(%s)\n{\n%s\n}\n\n",
            $name,
            $name,
            $renames[$factory] ?? $factory,
            factoryParams($class, $fieldTypes, $class->methodSignatures[$factory], false),
            $body
        );
    }

    $decodeBody = implode("\n", $decode->lines);
    $encodeBody = implode("\n", $encode->lines);

    $contents = licenseHeader("src/{$name}.php") . <<<CPP

#include "bedrock_protocol/protocol/{$name}.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

{$createImpl}void {$name}::decodePayload(encoding::ByteBufferReader &in)
{
{$decodeBody}
}

void {$name}::encodePayload(encoding::ByteBufferWriter &out) const
{
{$encodeBody}
}

bool {$name}::handle(PacketHandlerInterface &handler)
{
    return handler.{$handler}(*this);
}

}  // namespace bedrock_protocol

CPP;

    file_put_contents(SRC_DIR . "/{$name}.cpp", $contents);
}
