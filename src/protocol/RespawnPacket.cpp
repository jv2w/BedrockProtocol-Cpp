/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/RespawnPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RespawnPacket.h"

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

RespawnPacket RespawnPacket::create(math::Vector3 position, std::uint8_t respawnState, std::uint64_t actorRuntimeId)
{
    RespawnPacket result;
    result.position = std::move(position);
    result.respawnState = respawnState;
    result.actorRuntimeId = actorRuntimeId;
    return result;
}

void RespawnPacket::decodePayload(encoding::ByteBufferReader &in)
{
    position = serializer::CommonTypes::getVector3(in);
    respawnState = encoding::Byte::readUnsigned(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);

}

void RespawnPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putVector3(out, position);
    encoding::Byte::writeUnsigned(out, respawnState);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);

}

bool RespawnPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRespawn(*this);
}

}  // namespace bedrock_protocol
