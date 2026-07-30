/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/RemoveActorPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RemoveActorPacket.h"

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

RemoveActorPacket RemoveActorPacket::create(std::int64_t actorUniqueId)
{
    RemoveActorPacket result;
    result.actorUniqueId = actorUniqueId;
    return result;
}

void RemoveActorPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);

}

void RemoveActorPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);

}

bool RemoveActorPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRemoveActor(*this);
}

}  // namespace bedrock_protocol
