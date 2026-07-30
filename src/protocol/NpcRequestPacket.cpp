/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NpcRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/NpcRequestPacket.h"

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

NpcRequestPacket NpcRequestPacket::create(std::uint64_t actorRuntimeId, std::uint8_t requestType, std::string commandString, std::uint8_t actionIndex, std::string sceneName)
{
    NpcRequestPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.requestType = requestType;
    result.commandString = std::move(commandString);
    result.actionIndex = actionIndex;
    result.sceneName = std::move(sceneName);
    return result;
}

void NpcRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    requestType = encoding::Byte::readUnsigned(in);
    commandString = serializer::CommonTypes::getString(in);
    actionIndex = encoding::Byte::readUnsigned(in);
    sceneName = serializer::CommonTypes::getString(in);

}

void NpcRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::Byte::writeUnsigned(out, requestType);
    serializer::CommonTypes::putString(out, commandString);
    encoding::Byte::writeUnsigned(out, actionIndex);
    serializer::CommonTypes::putString(out, sceneName);

}

bool NpcRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleNpcRequest(*this);
}

}  // namespace bedrock_protocol
