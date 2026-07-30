/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PartyChangedPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PartyChangedPacket.h"

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

PartyChangedPacket PartyChangedPacket::create(std::string partyId, bool partyLeader)
{
    PartyChangedPacket result;
    result.partyId = std::move(partyId);
    result.partyLeader = partyLeader;
    return result;
}

void PartyChangedPacket::decodePayload(encoding::ByteBufferReader &in)
{
    partyId = serializer::CommonTypes::getString(in);
    partyLeader = serializer::CommonTypes::getBool(in);

}

void PartyChangedPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, partyId);
    serializer::CommonTypes::putBool(out, partyLeader);

}

bool PartyChangedPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePartyChanged(*this);
}

}  // namespace bedrock_protocol
