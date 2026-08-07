/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundMapItemDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundMapItemDataPacket.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {
/**
 * BEARGB: the four bytes B, G, R, A in that order.
 *
 * gophertunnel v1.58.0 minecraft/protocol/writer.go:166-169 packs A | R<<8 | G<<16 | B<<24 and writes
 * it big-endian, which is byte for byte what a little-endian write of toARGB() produces.
 */
color::Color getBeArgb(encoding::ByteBufferReader &in)
{
    return color::Color::fromARGB(encoding::LE::readUnsignedInt(in));
}

void putBeArgb(encoding::ByteBufferWriter &out, const color::Color &color)
{
    encoding::LE::writeUnsignedInt(out, color.toARGB());
}

/**
 * gophertunnel v1.58.0 minecraft/protocol/map.go:48-56. Both optionals are always on the wire, each
 * behind its own presence byte, independently of the type.
 */
types::MapTrackedObject getTrackedObject(encoding::ByteBufferReader &in)
{
    types::MapTrackedObject object;
    object.type = encoding::LE::readSignedInt(in);
    object.actorUniqueId = serializer::CommonTypes::getBool(in)
                               ? std::optional(serializer::CommonTypes::getActorUniqueId(in))
                               : std::nullopt;
    object.blockPosition = serializer::CommonTypes::getBool(in)
                               ? std::optional(serializer::CommonTypes::getBlockPosition(in))
                               : std::nullopt;
    if (object.type != types::MapTrackedObject::TYPE_ENTITY && object.type != types::MapTrackedObject::TYPE_BLOCK) {
        throw PacketDecodeException("Unknown map object type " + std::to_string(object.type));
    }
    return object;
}

void putTrackedObject(encoding::ByteBufferWriter &out, const types::MapTrackedObject &object)
{
    encoding::LE::writeSignedInt(out, object.type);
    serializer::CommonTypes::putBool(out, object.actorUniqueId.has_value());
    if (object.actorUniqueId.has_value()) {
        serializer::CommonTypes::putActorUniqueId(out, *object.actorUniqueId);
    }
    serializer::CommonTypes::putBool(out, object.blockPosition.has_value());
    if (object.blockPosition.has_value()) {
        serializer::CommonTypes::putBlockPosition(out, *object.blockPosition);
    }
    if (object.type != types::MapTrackedObject::TYPE_ENTITY && object.type != types::MapTrackedObject::TYPE_BLOCK) {
        throw std::invalid_argument("Unknown map object type " + std::to_string(object.type));
    }
}

/** gophertunnel v1.58.0 minecraft/protocol/map.go:78-86. */
types::MapDecoration getDecoration(encoding::ByteBufferReader &in)
{
    const auto icon = encoding::Byte::readUnsigned(in);
    const auto rotation = encoding::Byte::readUnsigned(in);
    const auto xOffset = encoding::Byte::readUnsigned(in);
    const auto yOffset = encoding::Byte::readUnsigned(in);
    auto label = serializer::CommonTypes::getString(in);
    const auto color = getBeArgb(in);

    return {icon, rotation, xOffset, yOffset, std::move(label), color};
}

void putDecoration(encoding::ByteBufferWriter &out, const types::MapDecoration &decoration)
{
    encoding::Byte::writeUnsigned(out, decoration.getIcon());
    encoding::Byte::writeUnsigned(out, decoration.getRotation());
    encoding::Byte::writeUnsigned(out, decoration.getXOffset());
    encoding::Byte::writeUnsigned(out, decoration.getYOffset());
    serializer::CommonTypes::putString(out, decoration.getLabel());
    putBeArgb(out, decoration.getColor());
}

std::optional<std::int32_t> getOptionalVarInt(encoding::ByteBufferReader &in)
{
    return serializer::CommonTypes::getBool(in) ? std::optional(encoding::VarInt::readSignedInt(in)) : std::nullopt;
}

void putOptionalVarInt(encoding::ByteBufferWriter &out, const std::optional<std::int32_t> &value)
{
    serializer::CommonTypes::putBool(out, value.has_value());
    if (value.has_value()) {
        encoding::VarInt::writeSignedInt(out, *value);
    }
}
}  // namespace

void ClientboundMapItemDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/packet/client_bound_map_item_data.go:61-83.
    mapId = encoding::VarInt::readSignedLong(in);
    dimensionId = encoding::Byte::readUnsigned(in);
    isLocked = serializer::CommonTypes::getBool(in);
    origin = serializer::CommonTypes::getBlockPosition(in);

    if (serializer::CommonTypes::getBool(in)) {
        std::vector<std::int64_t> ids;
        const auto count = encoding::VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < count; ++i) {
            ids.push_back(encoding::VarInt::readSignedLong(in));
        }
        parentMapIds = std::move(ids);
    }
    else {
        parentMapIds = std::nullopt;
    }

    scale = serializer::CommonTypes::getBool(in) ? std::optional(encoding::Byte::readUnsigned(in)) : std::nullopt;

    if (serializer::CommonTypes::getBool(in)) {
        std::vector<types::MapTrackedObject> objects;
        const auto count = encoding::VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < count; ++i) {
            objects.push_back(getTrackedObject(in));
        }
        trackedEntities = std::move(objects);
    }
    else {
        trackedEntities = std::nullopt;
    }

    if (serializer::CommonTypes::getBool(in)) {
        std::vector<types::MapDecoration> newDecorations;
        const auto count = encoding::VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < count; ++i) {
            newDecorations.push_back(getDecoration(in));
        }
        decorations = std::move(newDecorations);
    }
    else {
        decorations = std::nullopt;
    }

    width = getOptionalVarInt(in);
    height = getOptionalVarInt(in);
    xOffset = getOptionalVarInt(in);
    yOffset = getOptionalVarInt(in);

    if (serializer::CommonTypes::getBool(in)) {
        std::vector<color::Color> pixels;
        const auto count = encoding::VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < count; ++i) {
            pixels.push_back(getBeArgb(in));
        }
        colors = std::move(pixels);
    }
    else {
        colors = std::nullopt;
    }
}

void ClientboundMapItemDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedLong(out, mapId);
    encoding::Byte::writeUnsigned(out, dimensionId);
    serializer::CommonTypes::putBool(out, isLocked);
    serializer::CommonTypes::putBlockPosition(out, origin);

    serializer::CommonTypes::putBool(out, parentMapIds.has_value());
    if (parentMapIds.has_value()) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(parentMapIds->size()));
        for (const auto &parentMapId : *parentMapIds) {
            encoding::VarInt::writeSignedLong(out, parentMapId);
        }
    }

    serializer::CommonTypes::putBool(out, scale.has_value());
    if (scale.has_value()) {
        encoding::Byte::writeUnsigned(out, *scale);
    }

    serializer::CommonTypes::putBool(out, trackedEntities.has_value());
    if (trackedEntities.has_value()) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(trackedEntities->size()));
        for (const auto &object : *trackedEntities) {
            putTrackedObject(out, object);
        }
    }

    serializer::CommonTypes::putBool(out, decorations.has_value());
    if (decorations.has_value()) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(decorations->size()));
        for (const auto &decoration : *decorations) {
            putDecoration(out, decoration);
        }
    }

    putOptionalVarInt(out, width);
    putOptionalVarInt(out, height);
    putOptionalVarInt(out, xOffset);
    putOptionalVarInt(out, yOffset);

    serializer::CommonTypes::putBool(out, colors.has_value());
    if (colors.has_value()) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(colors->size()));
        for (const auto &pixel : *colors) {
            putBeArgb(out, pixel);
        }
    }
}

bool ClientboundMapItemDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundMapItemData(*this);
}

}  // namespace bedrock_protocol
