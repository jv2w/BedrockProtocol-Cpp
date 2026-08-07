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

#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

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
 * The variant name written directly after the variant ID, indexed by that ID.
 * @see gophertunnel minecraft/protocol/scoreboard.go:42
 */
constexpr std::string_view VARIANT_NAMES[] = {"remove", "changeplayer", "changeentity", "changefakeplayer"};

}  // namespace

SetScorePacket SetScorePacket::create(std::vector<types::ScorePacketEntry> entries)
{
    SetScorePacket result;
    result.entries = std::move(entries);
    return result;
}

void SetScorePacket::decodePayload(encoding::ByteBufferReader &in)
{
    for (std::uint32_t i = 0, i2 = encoding::VarInt::readUnsignedInt(in); i < i2; ++i) {
        types::ScorePacketEntry entry;
        const auto variant = encoding::VarInt::readUnsignedInt(in);
        if (variant >= std::size(VARIANT_NAMES)) {
            throw PacketDecodeException("Unknown entry type " + std::to_string(variant));
        }
        entry.type = static_cast<std::uint8_t>(variant);
        serializer::CommonTypes::getString(in);  // the variant name, redundant with the variant ID
        entry.scoreboardId = encoding::VarInt::readSignedLong(in);
        switch (entry.type) {
            case types::ScorePacketEntry::TYPE_REMOVE: {
                // The objective name is optional in this variant; an absent one means the empty string.
                // @see gophertunnel minecraft/protocol/scoreboard.go:55-60
                const auto objectiveName = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });
                entry.objectiveName = objectiveName.value_or("");
                break;
            }
            case types::ScorePacketEntry::TYPE_PLAYER:
            case types::ScorePacketEntry::TYPE_ENTITY:
                entry.objectiveName = serializer::CommonTypes::getString(in);
                entry.score = encoding::LE::readSignedInt(in);
                entry.actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
                break;
            case types::ScorePacketEntry::TYPE_FAKE_PLAYER:
                entry.objectiveName = serializer::CommonTypes::getString(in);
                entry.score = encoding::LE::readSignedInt(in);
                entry.customName = serializer::CommonTypes::getString(in);
                break;
            default:
                throw PacketDecodeException("Unknown entry type " + std::to_string(entry.type));
        }
        entries.push_back(std::move(entry));
    }

}

void SetScorePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        if (entry.type >= std::size(VARIANT_NAMES)) {
            throw std::invalid_argument("Unknown entry type " + std::to_string(entry.type));
        }
        encoding::VarInt::writeUnsignedInt(out, entry.type);
        serializer::CommonTypes::putString(out, std::string(VARIANT_NAMES[entry.type]));
        encoding::VarInt::writeSignedLong(out, entry.scoreboardId);
        switch (entry.type) {
            case types::ScorePacketEntry::TYPE_REMOVE: {
                const auto objectiveName = entry.objectiveName.empty() ? std::optional<std::string>() : std::optional<std::string>(entry.objectiveName);
                serializer::CommonTypes::writeOptional(out, objectiveName, [](encoding::ByteBufferWriter &writer, const std::string &value) { serializer::CommonTypes::putString(writer, value); });
                break;
            }
            case types::ScorePacketEntry::TYPE_PLAYER:
            case types::ScorePacketEntry::TYPE_ENTITY:
                serializer::CommonTypes::putString(out, entry.objectiveName);
                encoding::LE::writeSignedInt(out, entry.score);
                serializer::CommonTypes::putActorUniqueId(out, *entry.actorUniqueId);
                break;
            case types::ScorePacketEntry::TYPE_FAKE_PLAYER:
                serializer::CommonTypes::putString(out, entry.objectiveName);
                encoding::LE::writeSignedInt(out, entry.score);
                serializer::CommonTypes::putString(out, *entry.customName);
                break;
            default:
                throw std::invalid_argument("Unknown entry type " + std::to_string(entry.type));
        }
    }

}

bool SetScorePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetScore(*this);
}

}  // namespace bedrock_protocol
