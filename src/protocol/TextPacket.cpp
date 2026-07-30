/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/TextPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/TextPacket.h"

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

TextPacket TextPacket::messageOnly(std::uint8_t type, const std::string &message)
{
    TextPacket result;
    result.type = type;
    //TODO: HACK! Empty message crashes or bugs out client in 1.21.130
    result.message = message.empty() ? " " : message;
    return result;
}

TextPacket TextPacket::baseTranslation(std::uint8_t type, const std::string &key, const std::vector<std::string> &parameters)
{
    TextPacket result;
    result.type = type;
    result.needsTranslation = true;
    //TODO: HACK! Empty message crashes or bugs out client in 1.21.130
    result.message = key.empty() ? " " : key;
    result.parameters = parameters;
    return result;
}

TextPacket TextPacket::raw(const std::string &message)
{
    return messageOnly(TYPE_RAW, message);
}

TextPacket TextPacket::translation(const std::string &key, const std::vector<std::string> &parameters)
{
    return baseTranslation(TYPE_TRANSLATION, key, parameters);
}

TextPacket TextPacket::popup(const std::string &message)
{
    return messageOnly(TYPE_POPUP, message);
}

TextPacket TextPacket::translatedPopup(const std::string &key, const std::vector<std::string> &parameters)
{
    return baseTranslation(TYPE_POPUP, key, parameters);
}

TextPacket TextPacket::jukeboxPopup(const std::string &key, const std::vector<std::string> &parameters)
{
    return baseTranslation(TYPE_JUKEBOX_POPUP, key, parameters);
}

TextPacket TextPacket::tip(const std::string &message)
{
    return messageOnly(TYPE_TIP, message);
}

void TextPacket::decodePayload(encoding::ByteBufferReader &in)
{
    needsTranslation = serializer::CommonTypes::getBool(in);

    const auto category = encoding::Byte::readUnsigned(in);
    type = encoding::Byte::readUnsigned(in);
    switch (type) {
        case TYPE_CHAT:
        case TYPE_WHISPER:
        /** @noinspection PhpMissingBreakStatementInspection */
        case TYPE_ANNOUNCEMENT:
            if (category != CATEGORY_AUTHORED_MESSAGE) {
                throw PacketDecodeException("Decoded TextPacket has invalid structure: type " + std::to_string(type) + " requires category CATEGORY_AUTHORED_MESSAGE");
            }
            sourceName = serializer::CommonTypes::getString(in);
            message = serializer::CommonTypes::getString(in);
            break;
        case TYPE_RAW:
        case TYPE_TIP:
        case TYPE_SYSTEM:
        case TYPE_JSON_WHISPER:
        case TYPE_JSON:
        case TYPE_JSON_ANNOUNCEMENT:
            if (category != CATEGORY_MESSAGE_ONLY) {
                throw PacketDecodeException("Decoded TextPacket has invalid structure: type " + std::to_string(type) + " requires category CATEGORY_MESSAGE_ONLY");
            }
            message = serializer::CommonTypes::getString(in);
            break;
        case TYPE_TRANSLATION:
        case TYPE_POPUP:
        case TYPE_JUKEBOX_POPUP: {
            if (category != CATEGORY_MESSAGE_WITH_PARAMETERS) {
                throw PacketDecodeException("Decoded TextPacket has invalid structure: type " + std::to_string(type) + " requires category CATEGORY_MESSAGE_WITH_PARAMETERS");
            }
            message = serializer::CommonTypes::getString(in);
            const auto count = encoding::VarInt::readUnsignedInt(in);
            for (std::uint32_t i = 0; i < count; ++i) {
                parameters.push_back(serializer::CommonTypes::getString(in));
            }
            break;
        }
    }

    xboxUserId = serializer::CommonTypes::getString(in);
    platformChatId = serializer::CommonTypes::getString(in);
    filteredMessage = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });
}

void TextPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, needsTranslation);

    std::uint8_t category;
    switch (type) {
        case TYPE_RAW:
        case TYPE_TIP:
        case TYPE_SYSTEM:
        case TYPE_JSON_WHISPER:
        case TYPE_JSON_ANNOUNCEMENT:
        case TYPE_JSON:
            category = CATEGORY_MESSAGE_ONLY;
            break;

        case TYPE_CHAT:
        case TYPE_WHISPER:
        case TYPE_ANNOUNCEMENT:
            category = CATEGORY_AUTHORED_MESSAGE;
            break;

        case TYPE_TRANSLATION:
        case TYPE_POPUP:
        case TYPE_JUKEBOX_POPUP:
            category = CATEGORY_MESSAGE_WITH_PARAMETERS;
            break;

        default:
            throw std::logic_error("Invalid TextPacket type: " + std::to_string(type));
    }

    encoding::Byte::writeUnsigned(out, category);
    encoding::Byte::writeUnsigned(out, type);
    switch (type) {
        case TYPE_CHAT:
        case TYPE_WHISPER:
        /** @noinspection PhpMissingBreakStatementInspection */
        case TYPE_ANNOUNCEMENT:
            serializer::CommonTypes::putString(out, sourceName);
            [[fallthrough]];
        case TYPE_RAW:
        case TYPE_TIP:
        case TYPE_SYSTEM:
        case TYPE_JSON_WHISPER:
        case TYPE_JSON:
        case TYPE_JSON_ANNOUNCEMENT:
            serializer::CommonTypes::putString(out, message);
            break;

        case TYPE_TRANSLATION:
        case TYPE_POPUP:
        case TYPE_JUKEBOX_POPUP:
            serializer::CommonTypes::putString(out, message);
            encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(parameters.size()));
            for (const auto &p : parameters) {
                serializer::CommonTypes::putString(out, p);
            }
            break;
    }

    serializer::CommonTypes::putString(out, xboxUserId);
    serializer::CommonTypes::putString(out, platformChatId);
    serializer::CommonTypes::writeOptional(out, filteredMessage, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putString(writer, value); });
}

bool TextPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleText(*this);
}

}  // namespace bedrock_protocol
