/*
 * This file is part of BedrockProtocol for Endstone.
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

float MoveActorDeltaPacket::maybeReadCoord(std::int32_t flag, encoding::ByteBufferReader &in) const
{
    if ((flags & flag) != 0) {
        return encoding::LE::readFloat(in);
    }
    return 0;
}

float MoveActorDeltaPacket::maybeReadRotation(std::int32_t flag, encoding::ByteBufferReader &in) const
{
    if ((flags & flag) != 0) {
        return serializer::CommonTypes::getRotationByte(in);
    }
    return 0.0;
}

void MoveActorDeltaPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    flags = encoding::LE::readUnsignedShort(in);
    xPos = maybeReadCoord(FLAG_HAS_X, in);
    yPos = maybeReadCoord(FLAG_HAS_Y, in);
    zPos = maybeReadCoord(FLAG_HAS_Z, in);
    pitch = maybeReadRotation(FLAG_HAS_PITCH, in);
    yaw = maybeReadRotation(FLAG_HAS_YAW, in);
    headYaw = maybeReadRotation(FLAG_HAS_HEAD_YAW, in);

}

void MoveActorDeltaPacket::maybeWriteCoord(std::int32_t flag, float val, encoding::ByteBufferWriter &out) const
{
    if ((flags & flag) != 0) {
        encoding::LE::writeFloat(out, val);
    }
}

void MoveActorDeltaPacket::maybeWriteRotation(std::int32_t flag, float val, encoding::ByteBufferWriter &out) const
{
    if ((flags & flag) != 0) {
        serializer::CommonTypes::putRotationByte(out, val);
    }
}

void MoveActorDeltaPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::LE::writeUnsignedShort(out, flags);
    maybeWriteCoord(FLAG_HAS_X, xPos, out);
    maybeWriteCoord(FLAG_HAS_Y, yPos, out);
    maybeWriteCoord(FLAG_HAS_Z, zPos, out);
    maybeWriteRotation(FLAG_HAS_PITCH, pitch, out);
    maybeWriteRotation(FLAG_HAS_YAW, yaw, out);
    maybeWriteRotation(FLAG_HAS_HEAD_YAW, headYaw, out);

}

bool MoveActorDeltaPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMoveActorDelta(*this);
}

}  // namespace bedrock_protocol
