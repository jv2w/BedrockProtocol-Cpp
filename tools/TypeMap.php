<?php

/**
 * Type mapping tables shared by the generator and the payload transpiler.
 *
 * This encodes the rules written down in PORTING.md §3; if the two disagree, PORTING.md wins and
 * this file is the bug.
 */

declare(strict_types=1);

namespace BedrockProtocolPort;

final class TypeMap
{
    /**
     * Maps a fully-qualified PHP reader call to the C++ type it produces.
     *
     * This is the authoritative source of integer widths: PHP's `int` carries no width information,
     * so the wire function that fills a field decides the field's C++ type.
     */
    public const READER_TO_CPP = [
        'Byte::readUnsigned' => 'std::uint8_t',
        'Byte::readSigned' => 'std::int8_t',

        'LE::readUnsignedShort' => 'std::uint16_t',
        'LE::readSignedShort' => 'std::int16_t',
        'LE::readUnsignedInt' => 'std::uint32_t',
        'LE::readSignedInt' => 'std::int32_t',
        'LE::readUnsignedLong' => 'std::uint64_t',
        'LE::readSignedLong' => 'std::int64_t',
        'LE::readFloat' => 'float',
        'LE::readDouble' => 'double',

        'BE::readUnsignedShort' => 'std::uint16_t',
        'BE::readSignedShort' => 'std::int16_t',
        'BE::readUnsignedInt' => 'std::uint32_t',
        'BE::readSignedInt' => 'std::int32_t',
        'BE::readUnsignedLong' => 'std::uint64_t',
        'BE::readSignedLong' => 'std::int64_t',
        'BE::readFloat' => 'float',
        'BE::readDouble' => 'double',

        'VarInt::readUnsignedInt' => 'std::uint32_t',
        'VarInt::readSignedInt' => 'std::int32_t',
        'VarInt::readUnsignedLong' => 'std::uint64_t',
        'VarInt::readSignedLong' => 'std::int64_t',

        'CommonTypes::getString' => 'std::string',
        'CommonTypes::getBool' => 'bool',
        'CommonTypes::getUUID' => 'uuid::Uuid',
        'CommonTypes::getSkin' => 'types::skin::SkinData',
        'CommonTypes::getItemStackWithoutStackId' => 'types::inventory::ItemStack',
        'CommonTypes::getItemStackWrapper' => 'types::inventory::ItemStackWrapper',
        'CommonTypes::getNetworkItemStackDescriptor' => 'types::inventory::ItemStackWrapper',
        'CommonTypes::getRecipeIngredient' => 'types::recipe::RecipeIngredient',
        'CommonTypes::getActorUniqueId' => 'std::int64_t',
        'CommonTypes::getActorRuntimeId' => 'std::uint64_t',
        'CommonTypes::getBlockPosition' => 'types::BlockPosition',
        'CommonTypes::getVector3' => 'math::Vector3',
        'CommonTypes::getVector2' => 'math::Vector2',
        'CommonTypes::getRotationByte' => 'float',
        'CommonTypes::getEntityLink' => 'types::entity::EntityLink',
        'CommonTypes::getCommandOriginData' => 'types::command::CommandOriginData',
        'CommonTypes::getStructureSettings' => 'types::StructureSettings',
        'CommonTypes::getStructureEditorData' => 'types::StructureEditorData',
        'CommonTypes::getNbtRoot' => 'nbt::TreeRoot',
        'CommonTypes::getNbtCompoundRoot' => 'nbt::tag::CompoundTag',
        'CommonTypes::readRecipeNetId' => 'std::uint32_t',
        'CommonTypes::readCreativeItemNetId' => 'std::uint32_t',
        'CommonTypes::readItemStackNetIdVariant' => 'std::int32_t',
        'CommonTypes::readItemStackRequestId' => 'std::int32_t',
        'CommonTypes::readLegacyItemStackRequestId' => 'std::int32_t',
        'CommonTypes::readServerItemStackId' => 'std::int32_t',

        // Aggregate readers whose element type has to be spelled out by hand.
        'CommonTypes::getEntityMetadata' => 'types::entity::EntityMetadataCollection',
        'CommonTypes::getGameRules' => 'std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>>',
    ];

    /** Maps declared PHP class names to their C++ equivalent (namespace-qualified from bedrock_protocol). */
    public const CLASS_TO_CPP = [
        'Vector3' => 'math::Vector3',
        'Vector2' => 'math::Vector2',
        'Color' => 'color::Color',
        'UuidInterface' => 'uuid::Uuid',
        'Uuid' => 'uuid::Uuid',
        'CompoundTag' => 'nbt::tag::CompoundTag',
        'TreeRoot' => 'nbt::TreeRoot',
    ];

    /** Headers required by the C++ types above, keyed by the C++ type name. */
    public const CPP_TYPE_TO_INCLUDE = [
        'math::Vector3' => 'bedrock_protocol/math/Vector3.h',
        'math::Vector2' => 'bedrock_protocol/math/Vector2.h',
        'color::Color' => 'bedrock_protocol/color/Color.h',
        'uuid::Uuid' => 'bedrock_protocol/uuid/Uuid.h',
        'nbt::tag::CompoundTag' => 'bedrock_protocol/nbt/tag/CompoundTag.h',
        'nbt::TreeRoot' => 'bedrock_protocol/nbt/TreeRoot.h',
    ];

    /** PHP scalar type declarations that map straight across. */
    public const SCALAR_TO_CPP = [
        'string' => 'std::string',
        'bool' => 'bool',
        'float' => 'float',
    ];

    /**
     * Resolves a `types\...` PHP class name to its C++ namespace path.
     *
     * The generator learns the subdirectory from the `use` statement, e.g.
     * `use ...\protocol\types\inventory\ItemStack;` yields `types::inventory::ItemStack`.
     */
    public static function typesClassToCpp(string $phpFqn): ?string
    {
        if (!preg_match('#\\\\protocol\\\\types\\\\(.*)$#', $phpFqn, $m)) {
            return null;
        }
        return 'types::' . str_replace('\\', '::', $m[1]);
    }

    /** Returns the include path for a C++ type produced by typesClassToCpp(). */
    public static function typesCppToInclude(string $cppName): string
    {
        return 'bedrock_protocol/protocol/' . str_replace('::', '/', $cppName) . '.h';
    }
}
