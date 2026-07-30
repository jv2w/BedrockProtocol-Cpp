/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BlockEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BlockEventPacket.h"

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

BlockEventPacket BlockEventPacket::create(types::BlockPosition blockPosition, std::int32_t eventType, std::int32_t eventData)
{
    BlockEventPacket result;
    result.blockPosition = std::move(blockPosition);
    result.eventType = eventType;
    result.eventData = eventData;
    return result;
}

void BlockEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    eventType = encoding::VarInt::readSignedInt(in);
    eventData = encoding::VarInt::readSignedInt(in);

}

void BlockEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    encoding::VarInt::writeSignedInt(out, eventType);
    encoding::VarInt::writeSignedInt(out, eventData);

}

bool BlockEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBlockEvent(*this);
}

}  // namespace bedrock_protocol
