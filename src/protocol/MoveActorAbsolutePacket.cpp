/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MoveActorAbsolutePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MoveActorAbsolutePacket.h"

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

MoveActorAbsolutePacket MoveActorAbsolutePacket::create(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t flags)
{
    MoveActorAbsolutePacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.position = std::move(position);
    result.pitch = pitch;
    result.yaw = yaw;
    result.headYaw = headYaw;
    result.flags = flags;
    return result;
}

void MoveActorAbsolutePacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    flags = encoding::Byte::readUnsigned(in);
    position = serializer::CommonTypes::getVector3(in);
    pitch = serializer::CommonTypes::getRotationByte(in);
    yaw = serializer::CommonTypes::getRotationByte(in);
    headYaw = serializer::CommonTypes::getRotationByte(in);

}

void MoveActorAbsolutePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::Byte::writeUnsigned(out, flags);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putRotationByte(out, pitch);
    serializer::CommonTypes::putRotationByte(out, yaw);
    serializer::CommonTypes::putRotationByte(out, headYaw);

}

bool MoveActorAbsolutePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMoveActorAbsolute(*this);
}

}  // namespace bedrock_protocol
