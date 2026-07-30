/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PartyDestinationCookieResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PartyDestinationCookieResponsePacket.h"

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

PartyDestinationCookieResponsePacket PartyDestinationCookieResponsePacket::create(std::string cookie, bool accepted)
{
    PartyDestinationCookieResponsePacket result;
    result.cookie = std::move(cookie);
    result.accepted = accepted;
    return result;
}

void PartyDestinationCookieResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    cookie = serializer::CommonTypes::getString(in);
    accepted = serializer::CommonTypes::getBool(in);

}

void PartyDestinationCookieResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, cookie);
    serializer::CommonTypes::putBool(out, accepted);

}

bool PartyDestinationCookieResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePartyDestinationCookieResponse(*this);
}

}  // namespace bedrock_protocol
