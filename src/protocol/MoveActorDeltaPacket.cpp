/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MoveActorDeltaPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MoveActorDeltaPacket.h"

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

namespace {

std::optional<float> readOptionalCoord(encoding::ByteBufferReader &in)
{
    if (!serializer::CommonTypes::getBool(in)) {
        return std::nullopt;
    }
    return encoding::LE::readFloat(in);
}

std::optional<float> readOptionalRotation(encoding::ByteBufferReader &in)
{
    if (!serializer::CommonTypes::getBool(in)) {
        return std::nullopt;
    }
    return serializer::CommonTypes::getRotationByte(in);
}

void writeOptionalCoord(encoding::ByteBufferWriter &out, const std::optional<float> &value)
{
    serializer::CommonTypes::putBool(out, value.has_value());
    if (value.has_value()) {
        encoding::LE::writeFloat(out, *value);
    }
}

void writeOptionalRotation(encoding::ByteBufferWriter &out, const std::optional<float> &value)
{
    serializer::CommonTypes::putBool(out, value.has_value());
    if (value.has_value()) {
        serializer::CommonTypes::putRotationByte(out, *value);
    }
}

}  // namespace

void MoveActorDeltaPacket::decodePayload(encoding::ByteBufferReader &in)
{
    // move_actor_delta.go:41-51
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    xPos = readOptionalCoord(in);
    yPos = readOptionalCoord(in);
    zPos = readOptionalCoord(in);
    pitch = readOptionalRotation(in);
    yaw = readOptionalRotation(in);
    headYaw = readOptionalRotation(in);
    onGround = serializer::CommonTypes::getBool(in);
    forceMove = serializer::CommonTypes::getBool(in);
    forceMoveLocalEntity = serializer::CommonTypes::getBool(in);
    forceCompletion = serializer::CommonTypes::getBool(in);

}

void MoveActorDeltaPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    writeOptionalCoord(out, xPos);
    writeOptionalCoord(out, yPos);
    writeOptionalCoord(out, zPos);
    writeOptionalRotation(out, pitch);
    writeOptionalRotation(out, yaw);
    writeOptionalRotation(out, headYaw);
    serializer::CommonTypes::putBool(out, onGround);
    serializer::CommonTypes::putBool(out, forceMove);
    serializer::CommonTypes::putBool(out, forceMoveLocalEntity);
    serializer::CommonTypes::putBool(out, forceCompletion);

}

bool MoveActorDeltaPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMoveActorDelta(*this);
}

}  // namespace bedrock_protocol
