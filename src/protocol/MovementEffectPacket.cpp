/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MovementEffectPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MovementEffectPacket.h"

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

MovementEffectPacket MovementEffectPacket::create(std::uint64_t actorRuntimeId, types::MovementEffectType effectType, std::uint32_t duration, std::uint64_t tick)
{
    MovementEffectPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.effectType = std::move(effectType);
    result.duration = duration;
    result.tick = tick;
    return result;
}

void MovementEffectPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    //Both fields are signed varints: Endstone r26_u4 MovementEffectPacket.json gives varint32,
    //Mojang's json marks them int32 + Compression, and gophertunnel v1.58.0
    //minecraft/protocol/packet/movement_effect.go:35-36 writes Varint32 for each.
    effectType = types::MovementEffectTypeFromPacket(encoding::VarInt::readSignedInt(in));
    duration = static_cast<std::uint32_t>(encoding::VarInt::readSignedInt(in));
    tick = encoding::VarInt::readUnsignedLong(in);

}

void MovementEffectPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(effectType));
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(duration));
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool MovementEffectPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMovementEffect(*this);
}

}  // namespace bedrock_protocol
