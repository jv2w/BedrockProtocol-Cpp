/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/LevelEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LevelEventPacket.h"

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

LevelEventPacket LevelEventPacket::create(std::int32_t eventId, std::int32_t eventData, std::optional<math::Vector3> position)
{
    LevelEventPacket result;
    result.eventId = eventId;
    result.eventData = eventData;
    result.position = std::move(position);
    return result;
}

LevelEventPacket LevelEventPacket::standardParticle(std::int32_t particleId, std::int32_t data, std::optional<math::Vector3> position)
{
    return create(types::LevelEvent::ADD_PARTICLE_MASK | particleId, data, position);
}

void LevelEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    eventId = encoding::VarInt::readSignedInt(in);
    position = serializer::CommonTypes::getVector3(in);
    eventData = encoding::VarInt::readSignedInt(in);

}

void LevelEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, eventId);
    serializer::CommonTypes::putVector3Nullable(out, position);
    encoding::VarInt::writeSignedInt(out, eventData);

}

bool LevelEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLevelEvent(*this);
}

}  // namespace bedrock_protocol
