/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MobEffectPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MobEffectPacket.h"

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

MobEffectPacket MobEffectPacket::create(std::uint64_t actorRuntimeId, std::uint8_t eventId, std::int32_t effectId, std::int32_t amplifier, bool particles, std::int32_t duration, std::uint64_t tick, bool ambient)
{
    MobEffectPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.eventId = eventId;
    result.effectId = effectId;
    result.amplifier = amplifier;
    result.particles = particles;
    result.duration = duration;
    result.tick = tick;
    result.ambient = ambient;
    return result;
}

MobEffectPacket MobEffectPacket::add(std::uint64_t actorRuntimeId, bool replace, std::int32_t effectId, std::int32_t amplifier, bool particles, std::int32_t duration, std::uint64_t tick, bool ambient)
{
    return create(actorRuntimeId, replace ? EVENT_MODIFY : EVENT_ADD, effectId, amplifier, particles, duration, tick, ambient);
}

MobEffectPacket MobEffectPacket::remove(std::uint64_t actorRuntimeId, std::int32_t effectId, std::uint64_t tick)
{
    return create(actorRuntimeId, EVENT_REMOVE, effectId, 0, false, 0, tick, false);
}

void MobEffectPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    eventId = encoding::Byte::readUnsigned(in);
    effectId = encoding::VarInt::readSignedInt(in);
    amplifier = encoding::VarInt::readSignedInt(in);
    particles = serializer::CommonTypes::getBool(in);
    duration = encoding::VarInt::readSignedInt(in);
    tick = encoding::VarInt::readUnsignedLong(in);
    ambient = serializer::CommonTypes::getBool(in);

}

void MobEffectPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::Byte::writeUnsigned(out, eventId);
    encoding::VarInt::writeSignedInt(out, effectId);
    encoding::VarInt::writeSignedInt(out, amplifier);
    serializer::CommonTypes::putBool(out, particles);
    encoding::VarInt::writeSignedInt(out, duration);
    encoding::VarInt::writeUnsignedLong(out, tick);
    serializer::CommonTypes::putBool(out, ambient);

}

bool MobEffectPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMobEffect(*this);
}

}  // namespace bedrock_protocol
