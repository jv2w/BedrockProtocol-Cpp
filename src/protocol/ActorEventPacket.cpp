/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ActorEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ActorEventPacket.h"

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

ActorEventPacket ActorEventPacket::create(std::uint64_t actorRuntimeId, std::uint8_t eventId, std::int32_t eventData, std::optional<math::Vector3> firePosition)
{
    ActorEventPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.eventId = eventId;
    result.eventData = eventData;
    result.firePosition = std::move(firePosition);
    return result;
}

void ActorEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    eventId = encoding::Byte::readUnsigned(in);
    eventData = encoding::VarInt::readSignedInt(in);
    firePosition = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getVector3(reader); });

}

void ActorEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::Byte::writeUnsigned(out, eventId);
    encoding::VarInt::writeSignedInt(out, eventData);
    serializer::CommonTypes::writeOptional(out, firePosition, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putVector3(writer, value); });

}

bool ActorEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleActorEvent(*this);
}

}  // namespace bedrock_protocol
