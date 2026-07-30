/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MotionPredictionHintsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MotionPredictionHintsPacket.h"

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

MotionPredictionHintsPacket MotionPredictionHintsPacket::create(std::uint64_t actorRuntimeId, math::Vector3 motion, bool onGround)
{
    MotionPredictionHintsPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.motion = std::move(motion);
    result.onGround = onGround;
    return result;
}

void MotionPredictionHintsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    motion = serializer::CommonTypes::getVector3(in);
    onGround = serializer::CommonTypes::getBool(in);

}

void MotionPredictionHintsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putVector3(out, motion);
    serializer::CommonTypes::putBool(out, onGround);

}

bool MotionPredictionHintsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMotionPredictionHints(*this);
}

}  // namespace bedrock_protocol
