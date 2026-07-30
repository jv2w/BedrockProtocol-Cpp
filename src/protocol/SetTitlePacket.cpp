/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetTitlePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetTitlePacket.h"

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

SetTitlePacket SetTitlePacket::create(std::int32_t type, std::string text, std::int32_t fadeInTime, std::int32_t stayTime, std::int32_t fadeOutTime, std::string xuid, std::string platformOnlineId, std::string filteredTitleText)
{
    SetTitlePacket result;
    result.type = type;
    result.text = std::move(text);
    result.fadeInTime = fadeInTime;
    result.stayTime = stayTime;
    result.fadeOutTime = fadeOutTime;
    result.xuid = std::move(xuid);
    result.platformOnlineId = std::move(platformOnlineId);
    result.filteredTitleText = std::move(filteredTitleText);
    return result;
}

SetTitlePacket SetTitlePacket::makeType(std::int32_t type)
{
    SetTitlePacket result;
    result.type = type;
    return result;
}

SetTitlePacket SetTitlePacket::makeText(std::int32_t type, std::string text)
{
    auto result = makeType(type);
    result.text = std::move(text);
    return result;
}

SetTitlePacket SetTitlePacket::title(std::string text)
{
    return makeText(TYPE_SET_TITLE, text);
}

SetTitlePacket SetTitlePacket::subtitle(std::string text)
{
    return makeText(TYPE_SET_SUBTITLE, text);
}

SetTitlePacket SetTitlePacket::actionBarMessage(std::string text)
{
    return makeText(TYPE_SET_ACTIONBAR_MESSAGE, text);
}

SetTitlePacket SetTitlePacket::clearTitle()
{
    return makeType(TYPE_CLEAR_TITLE);
}

SetTitlePacket SetTitlePacket::resetTitleOptions()
{
    return makeType(TYPE_RESET_TITLE);
}

SetTitlePacket SetTitlePacket::setAnimationTimes(std::int32_t fadeIn, std::int32_t stay, std::int32_t fadeOut)
{
    auto result = makeType(TYPE_SET_ANIMATION_TIMES);
    result.fadeInTime = fadeIn;
    result.stayTime = stay;
    result.fadeOutTime = fadeOut;
    return result;
}

void SetTitlePacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::VarInt::readSignedInt(in);
    text = serializer::CommonTypes::getString(in);
    fadeInTime = encoding::VarInt::readSignedInt(in);
    stayTime = encoding::VarInt::readSignedInt(in);
    fadeOutTime = encoding::VarInt::readSignedInt(in);
    xuid = serializer::CommonTypes::getString(in);
    platformOnlineId = serializer::CommonTypes::getString(in);
    filteredTitleText = serializer::CommonTypes::getString(in);

}

void SetTitlePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, type);
    serializer::CommonTypes::putString(out, text);
    encoding::VarInt::writeSignedInt(out, fadeInTime);
    encoding::VarInt::writeSignedInt(out, stayTime);
    encoding::VarInt::writeSignedInt(out, fadeOutTime);
    serializer::CommonTypes::putString(out, xuid);
    serializer::CommonTypes::putString(out, platformOnlineId);
    serializer::CommonTypes::putString(out, filteredTitleText);

}

bool SetTitlePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetTitle(*this);
}

}  // namespace bedrock_protocol
