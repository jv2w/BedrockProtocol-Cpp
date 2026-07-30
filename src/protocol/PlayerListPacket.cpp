/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayerListPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerListPacket.h"

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

PlayerListPacket PlayerListPacket::create(std::uint8_t type, std::vector<types::PlayerListEntry> entries)
{
    PlayerListPacket result;
    result.type = type;
    result.entries = std::move(entries);
    return result;
}

PlayerListPacket PlayerListPacket::add(std::vector<types::PlayerListEntry> entries)
{
    return create(TYPE_ADD, std::move(entries));
}

PlayerListPacket PlayerListPacket::remove(std::vector<types::PlayerListEntry> entries)
{
    return create(TYPE_REMOVE, std::move(entries));
}

void PlayerListPacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::Byte::readUnsigned(in);
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; ++i) {
        types::PlayerListEntry entry;

        if (type == TYPE_ADD) {
            entry.uuid = serializer::CommonTypes::getUUID(in);
            entry.actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
            entry.username = serializer::CommonTypes::getString(in);
            entry.xboxUserId = serializer::CommonTypes::getString(in);
            entry.platformChatId = serializer::CommonTypes::getString(in);
            entry.buildPlatform = encoding::LE::readSignedInt(in);
            entry.skinData = serializer::CommonTypes::getSkin(in);
            entry.isTeacher = serializer::CommonTypes::getBool(in);
            entry.isHost = serializer::CommonTypes::getBool(in);
            entry.isSubClient = serializer::CommonTypes::getBool(in);
            entry.color = color::Color::fromARGB(encoding::LE::readUnsignedInt(in));
        }
        else {
            entry.uuid = serializer::CommonTypes::getUUID(in);
        }

        entries.push_back(std::move(entry));
    }
    if (type == TYPE_ADD) {
        for (std::uint32_t i = 0; i < count; ++i) {
            entries[i].skinData.value().setVerified(serializer::CommonTypes::getBool(in));
        }
    }

}

void PlayerListPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, type);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        if (type == TYPE_ADD) {
            serializer::CommonTypes::putUUID(out, entry.uuid);
            serializer::CommonTypes::putActorUniqueId(out, entry.actorUniqueId);
            serializer::CommonTypes::putString(out, entry.username);
            serializer::CommonTypes::putString(out, entry.xboxUserId);
            serializer::CommonTypes::putString(out, entry.platformChatId);
            encoding::LE::writeSignedInt(out, entry.buildPlatform);
            serializer::CommonTypes::putSkin(out, entry.skinData.value());
            serializer::CommonTypes::putBool(out, entry.isTeacher);
            serializer::CommonTypes::putBool(out, entry.isHost);
            serializer::CommonTypes::putBool(out, entry.isSubClient);
            encoding::LE::writeUnsignedInt(out, (entry.color.has_value() ? entry.color.value() : color::Color(255, 255, 255)).toARGB());
        }
        else {
            serializer::CommonTypes::putUUID(out, entry.uuid);
        }
    }
    if (type == TYPE_ADD) {
        for (const auto &entry : entries) {
            serializer::CommonTypes::putBool(out, entry.skinData.value().isVerified());
        }
    }

}

bool PlayerListPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerList(*this);
}

}  // namespace bedrock_protocol
