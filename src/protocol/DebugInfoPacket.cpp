/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/DebugInfoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/DebugInfoPacket.h"

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

DebugInfoPacket DebugInfoPacket::create(std::int64_t actorUniqueId, std::string data)
{
    DebugInfoPacket result;
    result.actorUniqueId = actorUniqueId;
    result.data = std::move(data);
    return result;
}

void DebugInfoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    data = serializer::CommonTypes::getString(in);

}

void DebugInfoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putString(out, data);

}

bool DebugInfoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleDebugInfo(*this);
}

}  // namespace bedrock_protocol
