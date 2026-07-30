/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetScorePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetScorePacket.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

SetScorePacket SetScorePacket::create(std::uint8_t type, std::vector<types::ScorePacketEntry> entries)
{
    SetScorePacket result;
    result.type = type;
    result.entries = std::move(entries);
    return result;
}

void SetScorePacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::Byte::readUnsigned(in);
    for (std::uint32_t i = 0, i2 = encoding::VarInt::readUnsignedInt(in); i < i2; ++i) {
        types::ScorePacketEntry entry;
        entry.scoreboardId = encoding::VarInt::readSignedLong(in);
        entry.objectiveName = serializer::CommonTypes::getString(in);
        entry.score = encoding::LE::readSignedInt(in);
        if (type != TYPE_REMOVE) {
            entry.type = encoding::Byte::readUnsigned(in);
            switch (entry.type) {
                case types::ScorePacketEntry::TYPE_PLAYER:
                case types::ScorePacketEntry::TYPE_ENTITY:
                    entry.actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
                    break;
                case types::ScorePacketEntry::TYPE_FAKE_PLAYER:
                    entry.customName = serializer::CommonTypes::getString(in);
                    break;
                default:
                    throw PacketDecodeException("Unknown entry type " + std::to_string(entry.type));
            }
        }
        entries.push_back(std::move(entry));
    }

}

void SetScorePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, type);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        encoding::VarInt::writeSignedLong(out, entry.scoreboardId);
        serializer::CommonTypes::putString(out, entry.objectiveName);
        encoding::LE::writeSignedInt(out, entry.score);
        if (type != TYPE_REMOVE) {
            encoding::Byte::writeUnsigned(out, entry.type);
            switch (entry.type) {
                case types::ScorePacketEntry::TYPE_PLAYER:
                case types::ScorePacketEntry::TYPE_ENTITY:
                    serializer::CommonTypes::putActorUniqueId(out, *entry.actorUniqueId);
                    break;
                case types::ScorePacketEntry::TYPE_FAKE_PLAYER:
                    serializer::CommonTypes::putString(out, *entry.customName);
                    break;
                default:
                    throw std::invalid_argument("Unknown entry type " + std::to_string(entry.type));
            }
        }
    }

}

bool SetScorePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetScore(*this);
}

}  // namespace bedrock_protocol
