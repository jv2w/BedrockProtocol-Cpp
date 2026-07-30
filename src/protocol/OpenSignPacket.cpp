/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/OpenSignPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/OpenSignPacket.h"

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

OpenSignPacket OpenSignPacket::create(types::BlockPosition blockPosition, bool front)
{
    OpenSignPacket result;
    result.blockPosition = std::move(blockPosition);
    result.front = front;
    return result;
}

void OpenSignPacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    front = serializer::CommonTypes::getBool(in);

}

void OpenSignPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    serializer::CommonTypes::putBool(out, front);

}

bool OpenSignPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleOpenSign(*this);
}

}  // namespace bedrock_protocol
