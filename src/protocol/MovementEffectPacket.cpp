/*
 * This file is part of BedrockProtocol for Endstone.
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
    effectType = types::MovementEffectTypeFromPacket(static_cast<std::int32_t>(encoding::VarInt::readUnsignedInt(in)));
    duration = encoding::VarInt::readUnsignedInt(in);
    tick = encoding::VarInt::readUnsignedLong(in);

}

void MovementEffectPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(effectType));
    encoding::VarInt::writeUnsignedInt(out, duration);
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool MovementEffectPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMovementEffect(*this);
}

}  // namespace bedrock_protocol
