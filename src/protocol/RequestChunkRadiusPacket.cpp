/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/RequestChunkRadiusPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RequestChunkRadiusPacket.h"

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

RequestChunkRadiusPacket RequestChunkRadiusPacket::create(std::int32_t radius, std::uint8_t maxRadius)
{
    RequestChunkRadiusPacket result;
    result.radius = radius;
    result.maxRadius = maxRadius;
    return result;
}

void RequestChunkRadiusPacket::decodePayload(encoding::ByteBufferReader &in)
{
    radius = encoding::VarInt::readSignedInt(in);
    maxRadius = encoding::Byte::readUnsigned(in);

}

void RequestChunkRadiusPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, radius);
    encoding::Byte::writeUnsigned(out, maxRadius);

}

bool RequestChunkRadiusPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRequestChunkRadius(*this);
}

}  // namespace bedrock_protocol
