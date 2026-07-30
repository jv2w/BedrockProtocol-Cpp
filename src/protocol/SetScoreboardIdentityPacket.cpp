/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetScoreboardIdentityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetScoreboardIdentityPacket.h"

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

SetScoreboardIdentityPacket SetScoreboardIdentityPacket::create(std::uint8_t type, std::vector<types::ScoreboardIdentityPacketEntry> entries)
{
    SetScoreboardIdentityPacket result;
    result.type = type;
    result.entries = std::move(entries);
    return result;
}

void SetScoreboardIdentityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::Byte::readUnsigned(in);
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        auto entry = types::ScoreboardIdentityPacketEntry();
        entry.scoreboardId = encoding::VarInt::readSignedLong(in);
        if (type == TYPE_REGISTER_IDENTITY) {
            entry.actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
        }

        entries.push_back(std::move(entry));
    }

}

void SetScoreboardIdentityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, type);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        encoding::VarInt::writeSignedLong(out, entry.scoreboardId);
        if (type == TYPE_REGISTER_IDENTITY) {
            serializer::CommonTypes::putActorUniqueId(out, *entry.actorUniqueId);
        }
    }

}

bool SetScoreboardIdentityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetScoreboardIdentity(*this);
}

}  // namespace bedrock_protocol
