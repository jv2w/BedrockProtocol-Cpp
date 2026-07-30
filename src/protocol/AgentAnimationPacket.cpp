/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AgentAnimationPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AgentAnimationPacket.h"

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

AgentAnimationPacket AgentAnimationPacket::create(std::uint8_t animationType, std::uint64_t actorRuntimeId)
{
    AgentAnimationPacket result;
    result.animationType = animationType;
    result.actorRuntimeId = actorRuntimeId;
    return result;
}

void AgentAnimationPacket::decodePayload(encoding::ByteBufferReader &in)
{
    animationType = encoding::Byte::readUnsigned(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);

}

void AgentAnimationPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, animationType);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);

}

bool AgentAnimationPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAgentAnimation(*this);
}

}  // namespace bedrock_protocol
