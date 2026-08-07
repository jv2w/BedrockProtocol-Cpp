/*
 * This file is part of BedrockProtocol-Cpp.
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

PlayerListPacket PlayerListPacket::create(std::vector<types::PlayerListEntry> entries)
{
    PlayerListPacket result;
    result.entries = std::move(entries);
    return result;
}

PlayerListPacket PlayerListPacket::add(std::vector<types::PlayerListEntry> entries)
{
    for (auto &entry : entries) {
        entry.actionType = types::PlayerListEntry::ACTION_ADD;
    }
    return create(std::move(entries));
}

PlayerListPacket PlayerListPacket::remove(std::vector<types::PlayerListEntry> entries)
{
    for (auto &entry : entries) {
        entry.actionType = types::PlayerListEntry::ACTION_REMOVE;
    }
    return create(std::move(entries));
}

void PlayerListPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; ++i) {
        types::PlayerListEntry entry;

        // player.go:115-130 - the action is per entry: a varuint32 variant (1 = Add, 0 = Remove)
        // decides it, followed by the legacy action byte, which is ignored.
        const auto variant = encoding::VarInt::readUnsignedInt(in);
        if (variant > 1) {
            throw PacketDecodeException("Unknown player list entry variant " + std::to_string(variant));
        }
        encoding::Byte::readUnsigned(in);
        entry.actionType =
            variant == 1 ? types::PlayerListEntry::ACTION_ADD : types::PlayerListEntry::ACTION_REMOVE;

        entry.uuid = serializer::CommonTypes::getUUID(in);
        if (entry.actionType == types::PlayerListEntry::ACTION_ADD) {
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
        // player.go:99-101 - a removal entry ends after its UUID.

        entries.push_back(std::move(entry));
    }
    // The trusted flag is no longer a trailing loop of its own: it lives inside the skin body
    // (player_list.go:23-25).

}

void PlayerListPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        // player.go:115-123 - the variant is 1 for Add and 0 for Remove, followed by the legacy
        // action byte.
        encoding::VarInt::writeUnsignedInt(out, entry.actionType == types::PlayerListEntry::ACTION_ADD ? 1 : 0);
        encoding::Byte::writeUnsigned(out, entry.actionType);

        serializer::CommonTypes::putUUID(out, entry.uuid);
        if (entry.actionType == types::PlayerListEntry::ACTION_ADD) {
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
        // player.go:99-101 - a removal entry ends after its UUID.
    }

}

bool PlayerListPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerList(*this);
}

}  // namespace bedrock_protocol
