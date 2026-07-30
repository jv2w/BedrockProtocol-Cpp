/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MovePlayerPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MovePlayerPacket.h"

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

MovePlayerPacket MovePlayerPacket::create(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t mode, bool onGround, std::uint64_t ridingActorRuntimeId, std::int32_t teleportCause, std::int32_t teleportItem, std::uint64_t tick)
{
    MovePlayerPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.position = std::move(position);
    result.pitch = pitch;
    result.yaw = yaw;
    result.headYaw = headYaw;
    result.mode = mode;
    result.onGround = onGround;
    result.ridingActorRuntimeId = ridingActorRuntimeId;
    result.teleportCause = teleportCause;
    result.teleportItem = teleportItem;
    result.tick = tick;
    return result;
}

MovePlayerPacket MovePlayerPacket::simple(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t mode, bool onGround, std::uint64_t ridingActorRuntimeId, std::uint64_t tick)
{
    return create(actorRuntimeId, position, pitch, yaw, headYaw, mode, onGround, ridingActorRuntimeId, 0, 0, tick);
}

void MovePlayerPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    position = serializer::CommonTypes::getVector3(in);
    pitch = encoding::LE::readFloat(in);
    yaw = encoding::LE::readFloat(in);
    headYaw = encoding::LE::readFloat(in);
    mode = encoding::Byte::readUnsigned(in);
    onGround = serializer::CommonTypes::getBool(in);
    ridingActorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    if (mode == MovePlayerPacket::MODE_TELEPORT) {
        teleportCause = encoding::LE::readSignedInt(in);
        teleportItem = encoding::LE::readSignedInt(in);
    }
    tick = encoding::VarInt::readUnsignedLong(in);

}

void MovePlayerPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putVector3(out, position);
    encoding::LE::writeFloat(out, pitch);
    encoding::LE::writeFloat(out, yaw);
    encoding::LE::writeFloat(out, headYaw); //TODO
    encoding::Byte::writeUnsigned(out, mode);
    serializer::CommonTypes::putBool(out, onGround);
    serializer::CommonTypes::putActorRuntimeId(out, ridingActorRuntimeId);
    if (mode == MovePlayerPacket::MODE_TELEPORT) {
        encoding::LE::writeSignedInt(out, teleportCause);
        encoding::LE::writeSignedInt(out, teleportItem);
    }
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool MovePlayerPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMovePlayer(*this);
}

}  // namespace bedrock_protocol
