/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetActorMotionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetActorMotionPacket.h"

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

SetActorMotionPacket SetActorMotionPacket::create(std::uint64_t actorRuntimeId, math::Vector3 motion, std::uint64_t tick)
{
    SetActorMotionPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.motion = std::move(motion);
    result.tick = tick;
    return result;
}

void SetActorMotionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    motion = serializer::CommonTypes::getVector3(in);
    tick = encoding::VarInt::readUnsignedLong(in);

}

void SetActorMotionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putVector3(out, motion);
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool SetActorMotionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetActorMotion(*this);
}

}  // namespace bedrock_protocol
