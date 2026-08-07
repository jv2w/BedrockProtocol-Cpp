/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/CommonTypes.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/nbt/TreeRoot.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/GameRule.h"
#include "bedrock_protocol/protocol/types/StructureEditorData.h"
#include "bedrock_protocol/protocol/types/StructureSettings.h"
#include "bedrock_protocol/protocol/types/command/CommandOriginData.h"
#include "bedrock_protocol/protocol/types/entity/EntityLink.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"
#include "bedrock_protocol/protocol/types/inventory/StackRequestItem.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::serializer {

/**
 * Serialization of the composite types shared by many packets.
 *
 * Every function takes the reader or writer as its first argument, mirroring the PHP original. The
 * class is never instantiated.
 */
class CommonTypes final {
public:
    CommonTypes() = delete;

    /** Entity metadata as it appears on the wire: property key, then the typed value, in order. */
    using EntityMetadata = std::vector<std::pair<std::uint32_t, std::unique_ptr<types::entity::MetadataProperty>>>;

    /** Game rules as they appear on the wire: rule name, then the typed value, in order. */
    using GameRules = std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>>;

    /**
     * Assigns into a list that stands in for a PHP keyed array.
     *
     * PHP decodes these collections with `$data[$key] = $value`, so a key that arrives twice
     * replaces the earlier entry where it already sits. A vector of pairs would keep both, which is
     * a different packet for any peer that repeats a key - something a client controls. The lookup
     * is linear, and these collections are small enough that it does not matter.
     */
    template <typename List, typename Key, typename Value>
    static void assignKeyed(List &list, const Key &key, Value &&value)
    {
        const auto existing = std::find_if(list.begin(), list.end(),
                                           [&key](const auto &entry) { return entry.first == key; });
        if (existing != list.end()) {
            existing->second = std::forward<Value>(value);
            return;
        }
        list.emplace_back(key, std::forward<Value>(value));
    }

    /** @throws DataDecodeException */
    static std::string getString(encoding::ByteBufferReader &in);
    static void putString(encoding::ByteBufferWriter &out, std::string_view v);

    /** @throws DataDecodeException */
    static bool getBool(encoding::ByteBufferReader &in);
    static void putBool(encoding::ByteBufferWriter &out, bool v);

    /** @throws DataDecodeException */
    static uuid::Uuid getUUID(encoding::ByteBufferReader &in);
    static void putUUID(encoding::ByteBufferWriter &out, const uuid::Uuid &uuid);

    /** @throws DataDecodeException */
    static types::skin::SkinData getSkin(encoding::ByteBufferReader &in);
    static void putSkin(encoding::ByteBufferWriter &out, const types::skin::SkinData &skin);

    /** @throws DataDecodeException */
    static types::inventory::ItemStack getItemStackWithoutStackId(encoding::ByteBufferReader &in);
    static void putItemStackWithoutStackId(encoding::ByteBufferWriter &out, const types::inventory::ItemStack &itemStack);

    /** @throws DataDecodeException */
    static types::inventory::ItemStackWrapper getNetworkItemStackDescriptor(encoding::ByteBufferReader &in);
    static void putNetworkItemStackDescriptor(encoding::ByteBufferWriter &out,
                                              const types::inventory::ItemStackWrapper &itemStackWrapper);

    /**
     * The name-based item format used by the deprecated craft-result stack request action.
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static types::inventory::StackRequestItem getStackRequestItem(encoding::ByteBufferReader &in);
    static void putStackRequestItem(encoding::ByteBufferWriter &out, const types::inventory::StackRequestItem &item);

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static types::recipe::RecipeIngredient getRecipeIngredient(encoding::ByteBufferReader &in);
    static void putRecipeIngredient(encoding::ByteBufferWriter &out, const types::recipe::RecipeIngredient &ingredient);

    /**
     * The tagless descriptor framing used by the auto-craft stack request action.
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static types::recipe::RecipeIngredient getStackRequestRecipeIngredient(encoding::ByteBufferReader &in);
    static void putStackRequestRecipeIngredient(encoding::ByteBufferWriter &out,
                                                const types::recipe::RecipeIngredient &ingredient);

    /** @throws DataDecodeException */
    static EntityMetadata getEntityMetadata(encoding::ByteBufferReader &in);
    static void putEntityMetadata(encoding::ByteBufferWriter &out, const EntityMetadata &metadata);

    /** @throws DataDecodeException */
    static std::int64_t getActorUniqueId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readSignedLong(in);
    }

    static void putActorUniqueId(encoding::ByteBufferWriter &out, std::int64_t eid)
    {
        encoding::VarInt::writeSignedLong(out, eid);
    }

    /** @throws DataDecodeException */
    static std::uint64_t getActorRuntimeId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readUnsignedLong(in);
    }

    static void putActorRuntimeId(encoding::ByteBufferWriter &out, std::uint64_t eid)
    {
        encoding::VarInt::writeUnsignedLong(out, eid);
    }

    /**
     * Reads a block position
     *
     * @throws DataDecodeException
     */
    static types::BlockPosition getBlockPosition(encoding::ByteBufferReader &in);

    /**
     * Writes a block position
     */
    static void putBlockPosition(encoding::ByteBufferWriter &out, const types::BlockPosition &blockPosition);

    /**
     * Reads a floating-point Vector3 object with coordinates rounded to 4 decimal places.
     *
     * @throws DataDecodeException
     */
    static math::Vector3 getVector3(encoding::ByteBufferReader &in)
    {
        const auto x = encoding::LE::readFloat(in);
        const auto y = encoding::LE::readFloat(in);
        const auto z = encoding::LE::readFloat(in);
        return {x, y, z};
    }

    /**
     * Reads a floating-point Vector2 object with coordinates rounded to 4 decimal places.
     *
     * @throws DataDecodeException
     */
    static math::Vector2 getVector2(encoding::ByteBufferReader &in)
    {
        const auto x = encoding::LE::readFloat(in);
        const auto y = encoding::LE::readFloat(in);
        return {x, y};
    }

    /**
     * Writes a floating-point Vector3 object, or 3x zero if null is given.
     *
     * Note: ONLY use this where it is reasonable to allow not specifying the vector.
     * For all other purposes, use the non-nullable version.
     *
     * @see CommonTypes::putVector3()
     */
    static void putVector3Nullable(encoding::ByteBufferWriter &out, const std::optional<math::Vector3> &vector)
    {
        if (vector.has_value()) {
            putVector3(out, *vector);
        }
        else {
            encoding::LE::writeFloat(out, 0.0F);
            encoding::LE::writeFloat(out, 0.0F);
            encoding::LE::writeFloat(out, 0.0F);
        }
    }

    /**
     * Writes a floating-point Vector3 object
     */
    static void putVector3(encoding::ByteBufferWriter &out, const math::Vector3 &vector)
    {
        encoding::LE::writeFloat(out, vector.x);
        encoding::LE::writeFloat(out, vector.y);
        encoding::LE::writeFloat(out, vector.z);
    }

    /**
     * Writes a floating-point Vector2 object
     */
    static void putVector2(encoding::ByteBufferWriter &out, const math::Vector2 &vector2)
    {
        encoding::LE::writeFloat(out, vector2.x);
        encoding::LE::writeFloat(out, vector2.y);
    }

    /** @throws DataDecodeException */
    static float getRotationByte(encoding::ByteBufferReader &in)
    {
        return static_cast<float>(encoding::Byte::readUnsigned(in)) * (360.0F / 256.0F);
    }

    static void putRotationByte(encoding::ByteBufferWriter &out, float rotation)
    {
        // PHP does `(int) ($rotation / (360 / 256))`, which truncates towards zero and may be negative
        // (pitch is routinely negative); the byte written is then the low 8 bits of that value.
        // Converting a negative float straight to std::uint8_t is undefined behaviour in C++, so
        // truncate to a signed integer first and let the well-defined modular narrowing produce the byte.
        const auto truncated = static_cast<std::int32_t>(rotation / (360.0F / 256.0F));
        encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(static_cast<std::uint32_t>(truncated) & 0xFFU));
    }

    /** @throws DataDecodeException */
    static GameRules getGameRules(encoding::ByteBufferReader &in);
    static void putGameRules(encoding::ByteBufferWriter &out, const GameRules &rules);

    /** @throws DataDecodeException */
    static types::entity::EntityLink getEntityLink(encoding::ByteBufferReader &in);
    static void putEntityLink(encoding::ByteBufferWriter &out, const types::entity::EntityLink &link);

    /** @throws DataDecodeException */
    static types::command::CommandOriginData getCommandOriginData(encoding::ByteBufferReader &in);
    static void putCommandOriginData(encoding::ByteBufferWriter &out, const types::command::CommandOriginData &data);

    /** @throws DataDecodeException */
    static types::StructureSettings getStructureSettings(encoding::ByteBufferReader &in);
    static void putStructureSettings(encoding::ByteBufferWriter &out, const types::StructureSettings &structureSettings);

    /** @throws DataDecodeException */
    static types::StructureEditorData getStructureEditorData(encoding::ByteBufferReader &in);
    static void putStructureEditorData(encoding::ByteBufferWriter &out,
                                       const types::StructureEditorData &structureEditorData);

    /** @throws PacketDecodeException */
    static nbt::TreeRoot getNbtRoot(encoding::ByteBufferReader &in);

    /** @throws PacketDecodeException */
    static nbt::tag::CompoundTag getNbtCompoundRoot(encoding::ByteBufferReader &in);

    /** @throws DataDecodeException */
    static std::uint32_t readRecipeNetId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readUnsignedInt(in);
    }

    static void writeRecipeNetId(encoding::ByteBufferWriter &out, std::uint32_t id)
    {
        encoding::VarInt::writeUnsignedInt(out, id);
    }

    /** @throws DataDecodeException */
    static std::uint32_t readCreativeItemNetId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readUnsignedInt(in);
    }

    static void writeCreativeItemNetId(encoding::ByteBufferWriter &out, std::uint32_t id)
    {
        encoding::VarInt::writeUnsignedInt(out, id);
    }

    /** @throws DataDecodeException */
    static std::int32_t readItemStackRequestId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readSignedInt(in);
    }

    static void writeItemStackRequestId(encoding::ByteBufferWriter &out, std::int32_t id)
    {
        encoding::VarInt::writeSignedInt(out, id);
    }

    /** @throws DataDecodeException */
    static std::int32_t readLegacyItemStackRequestId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readSignedInt(in);
    }

    static void writeLegacyItemStackRequestId(encoding::ByteBufferWriter &out, std::int32_t id)
    {
        encoding::VarInt::writeSignedInt(out, id);
    }

    /** @throws DataDecodeException */
    static std::int32_t readServerItemStackId(encoding::ByteBufferReader &in)
    {
        return encoding::VarInt::readSignedInt(in);
    }

    static void writeServerItemStackId(encoding::ByteBufferWriter &out, std::int32_t id)
    {
        encoding::VarInt::writeSignedInt(out, id);
    }

    /**
     * Reads a value that is present only when a preceding boolean says so.
     *
     * PHP passes a Closure; the C++ port takes any callable with the same shape.
     *
     * @throws DataDecodeException
     */
    template <typename Reader>
    static auto readOptional(encoding::ByteBufferReader &in, Reader reader)
        -> std::optional<decltype(reader(in))>
    {
        if (getBool(in)) {
            return reader(in);
        }
        return std::nullopt;
    }

    /** Writes a presence boolean, followed by the value itself when there is one. */
    template <typename T, typename Writer>
    static void writeOptional(encoding::ByteBufferWriter &out, const std::optional<T> &value, Writer writer)
    {
        if (value.has_value()) {
            putBool(out, true);
            writer(out, *value);
        }
        else {
            putBool(out, false);
        }
    }
};

}  // namespace bedrock_protocol::serializer
