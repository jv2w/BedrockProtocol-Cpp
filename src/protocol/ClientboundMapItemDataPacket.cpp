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

#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Endian.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {
/** Stands in for PHP's Binary::flipIntEndianness(), which the C++ port has no utils\Binary equivalent for. */
std::uint32_t flipIntEndianness(const std::uint32_t value)
{
    return encoding::detail::byteSwap<std::uint32_t>(value);
}
}  // namespace

void ClientboundMapItemDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    mapId = serializer::CommonTypes::getActorUniqueId(in);
    type = encoding::VarInt::readUnsignedInt(in);
    dimensionId = encoding::Byte::readUnsigned(in);
    isLocked = serializer::CommonTypes::getBool(in);
    origin = serializer::CommonTypes::getBlockPosition(in);

    if ((type & BITFLAG_MAP_CREATION) != 0) {
        const auto count = encoding::VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < count; ++i) {
            parentMapIds.push_back(serializer::CommonTypes::getActorUniqueId(in));
        }
    }

    if ((type & (BITFLAG_MAP_CREATION | BITFLAG_DECORATION_UPDATE | BITFLAG_TEXTURE_UPDATE)) != 0) { //Decoration bitflag or colour bitflag
        scale = encoding::Byte::readUnsigned(in);
    }

    if ((type & BITFLAG_DECORATION_UPDATE) != 0) {
        for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
            types::MapTrackedObject object;
            object.type = encoding::LE::readUnsignedInt(in);
            if (object.type == types::MapTrackedObject::TYPE_BLOCK) {
                object.blockPosition = serializer::CommonTypes::getBlockPosition(in);
            }
            else if (object.type == types::MapTrackedObject::TYPE_ENTITY) {
                object.actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
            }
            else {
                throw PacketDecodeException("Unknown map object type " + std::to_string(object.type));
            }
            trackedEntities.push_back(object);
        }

        for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
            const auto icon = encoding::Byte::readUnsigned(in);
            const auto rotation = encoding::Byte::readUnsigned(in);
            const auto decorationXOffset = encoding::Byte::readUnsigned(in);
            const auto decorationYOffset = encoding::Byte::readUnsigned(in);
            const auto label = serializer::CommonTypes::getString(in);
            const auto color = color::Color::fromRGBA(flipIntEndianness(encoding::VarInt::readUnsignedInt(in)));
            decorations.push_back(types::MapDecoration(icon, rotation, decorationXOffset, decorationYOffset, label, color));
        }
    }

    if ((type & BITFLAG_TEXTURE_UPDATE) != 0) {
        const auto width = encoding::VarInt::readSignedInt(in);
        const auto height = encoding::VarInt::readSignedInt(in);
        xOffset = encoding::VarInt::readSignedInt(in);
        yOffset = encoding::VarInt::readSignedInt(in);

        const auto count = encoding::VarInt::readUnsignedInt(in);
        if (static_cast<std::int64_t>(count) != static_cast<std::int64_t>(width) * height) {
            throw PacketDecodeException("Expected colour count of " + std::to_string(static_cast<std::int64_t>(height) * width) + " (height " + std::to_string(height) + " * width " + std::to_string(width) + "), got " + std::to_string(count));
        }

        colors = types::MapImage::decode(in, height, width);
    }
}

void ClientboundMapItemDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, mapId);

    std::uint32_t type = 0;
    const auto parentMapIdsCount = static_cast<std::uint32_t>(parentMapIds.size());
    if (parentMapIdsCount > 0) {
        type |= BITFLAG_MAP_CREATION;
    }
    const auto decorationCount = static_cast<std::uint32_t>(decorations.size());
    if (decorationCount > 0) {
        type |= BITFLAG_DECORATION_UPDATE;
    }
    if (colors.has_value()) {
        type |= BITFLAG_TEXTURE_UPDATE;
    }

    encoding::VarInt::writeUnsignedInt(out, type);
    encoding::Byte::writeUnsigned(out, dimensionId);
    serializer::CommonTypes::putBool(out, isLocked);
    serializer::CommonTypes::putBlockPosition(out, origin);

    if ((type & BITFLAG_MAP_CREATION) != 0) {
        encoding::VarInt::writeUnsignedInt(out, parentMapIdsCount);
        for (const auto &parentMapId : parentMapIds) {
            serializer::CommonTypes::putActorUniqueId(out, parentMapId);
        }
    }

    if ((type & (BITFLAG_MAP_CREATION | BITFLAG_TEXTURE_UPDATE | BITFLAG_DECORATION_UPDATE)) != 0) {
        encoding::Byte::writeUnsigned(out, scale);
    }

    if ((type & BITFLAG_DECORATION_UPDATE) != 0) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(trackedEntities.size()));
        for (const auto &object : trackedEntities) {
            encoding::LE::writeUnsignedInt(out, object.type);
            if (object.type == types::MapTrackedObject::TYPE_BLOCK) {
                serializer::CommonTypes::putBlockPosition(out, object.blockPosition);
            }
            else if (object.type == types::MapTrackedObject::TYPE_ENTITY) {
                serializer::CommonTypes::putActorUniqueId(out, object.actorUniqueId);
            }
            else {
                throw std::invalid_argument("Unknown map object type " + std::to_string(object.type));
            }
        }

        encoding::VarInt::writeUnsignedInt(out, decorationCount);
        for (const auto &decoration : decorations) {
            encoding::Byte::writeUnsigned(out, decoration.getIcon());
            encoding::Byte::writeUnsigned(out, decoration.getRotation());
            encoding::Byte::writeUnsigned(out, decoration.getXOffset());
            encoding::Byte::writeUnsigned(out, decoration.getYOffset());
            serializer::CommonTypes::putString(out, decoration.getLabel());
            encoding::VarInt::writeUnsignedInt(out, flipIntEndianness(decoration.getColor().toRGBA()));
        }
    }

    if (colors.has_value()) {
        encoding::VarInt::writeSignedInt(out, colors->getWidth());
        encoding::VarInt::writeSignedInt(out, colors->getHeight());
        encoding::VarInt::writeSignedInt(out, xOffset);
        encoding::VarInt::writeSignedInt(out, yOffset);

        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(colors->getWidth() * colors->getHeight())); //list count, but we handle it as a 2D array... thanks for the confusion mojang

        colors->encode(out);
    }
}

bool ClientboundMapItemDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundMapItemData(*this);
}

}  // namespace bedrock_protocol
