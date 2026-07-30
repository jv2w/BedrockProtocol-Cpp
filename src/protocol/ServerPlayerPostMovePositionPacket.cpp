/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerPlayerPostMovePositionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerPlayerPostMovePositionPacket.h"

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

ServerPlayerPostMovePositionPacket ServerPlayerPostMovePositionPacket::create(math::Vector3 position)
{
    ServerPlayerPostMovePositionPacket result;
    result.position = std::move(position);
    return result;
}

void ServerPlayerPostMovePositionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    position = serializer::CommonTypes::getVector3(in);

}

void ServerPlayerPostMovePositionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putVector3(out, position);

}

bool ServerPlayerPostMovePositionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerPlayerPostMovePosition(*this);
}

}  // namespace bedrock_protocol
