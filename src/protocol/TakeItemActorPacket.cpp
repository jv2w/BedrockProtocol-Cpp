/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/TakeItemActorPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/TakeItemActorPacket.h"

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

TakeItemActorPacket TakeItemActorPacket::create(std::uint64_t takerActorRuntimeId, std::uint64_t itemActorRuntimeId)
{
    TakeItemActorPacket result;
    result.takerActorRuntimeId = takerActorRuntimeId;
    result.itemActorRuntimeId = itemActorRuntimeId;
    return result;
}

void TakeItemActorPacket::decodePayload(encoding::ByteBufferReader &in)
{
    itemActorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    takerActorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);

}

void TakeItemActorPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, itemActorRuntimeId);
    serializer::CommonTypes::putActorRuntimeId(out, takerActorRuntimeId);

}

bool TakeItemActorPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleTakeItemActor(*this);
}

}  // namespace bedrock_protocol
