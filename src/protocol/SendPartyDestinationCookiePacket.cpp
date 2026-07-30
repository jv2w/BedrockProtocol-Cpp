/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SendPartyDestinationCookiePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SendPartyDestinationCookiePacket.h"

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

SendPartyDestinationCookiePacket SendPartyDestinationCookiePacket::create(std::string cookie, std::string intent, std::string destinationName)
{
    SendPartyDestinationCookiePacket result;
    result.cookie = std::move(cookie);
    result.intent = std::move(intent);
    result.destinationName = std::move(destinationName);
    return result;
}

void SendPartyDestinationCookiePacket::decodePayload(encoding::ByteBufferReader &in)
{
    cookie = serializer::CommonTypes::getString(in);
    intent = serializer::CommonTypes::getString(in);
    destinationName = serializer::CommonTypes::getString(in);

}

void SendPartyDestinationCookiePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, cookie);
    serializer::CommonTypes::putString(out, intent);
    serializer::CommonTypes::putString(out, destinationName);

}

bool SendPartyDestinationCookiePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSendPartyDestinationCookie(*this);
}

}  // namespace bedrock_protocol
