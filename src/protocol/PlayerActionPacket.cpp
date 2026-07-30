/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerActionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerActionPacket.h"

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

PlayerActionPacket PlayerActionPacket::create(std::uint64_t actorRuntimeId, std::int32_t action, types::BlockPosition blockPosition, types::BlockPosition resultPosition, std::int32_t face)
{
    PlayerActionPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.action = action;
    result.blockPosition = std::move(blockPosition);
    result.resultPosition = std::move(resultPosition);
    result.face = face;
    return result;
}

void PlayerActionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    action = encoding::VarInt::readSignedInt(in);
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    resultPosition = serializer::CommonTypes::getBlockPosition(in);
    face = encoding::VarInt::readSignedInt(in);

}

void PlayerActionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeSignedInt(out, action);
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    serializer::CommonTypes::putBlockPosition(out, resultPosition);
    encoding::VarInt::writeSignedInt(out, face);

}

bool PlayerActionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerAction(*this);
}

}  // namespace bedrock_protocol
