/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/NetworkStackLatencyPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/NetworkStackLatencyPacket.h"

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

NetworkStackLatencyPacket NetworkStackLatencyPacket::create(std::uint64_t timestamp, bool needResponse)
{
    NetworkStackLatencyPacket result;
    result.timestamp = timestamp;
    result.needResponse = needResponse;
    return result;
}

NetworkStackLatencyPacket NetworkStackLatencyPacket::request(std::uint64_t timestamp)
{
    return create(timestamp, true);
}

NetworkStackLatencyPacket NetworkStackLatencyPacket::response(std::uint64_t timestamp)
{
    return create(timestamp, false);
}

void NetworkStackLatencyPacket::decodePayload(encoding::ByteBufferReader &in)
{
    timestamp = encoding::LE::readUnsignedLong(in);
    needResponse = serializer::CommonTypes::getBool(in);

}

void NetworkStackLatencyPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedLong(out, timestamp);
    serializer::CommonTypes::putBool(out, needResponse);

}

bool NetworkStackLatencyPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleNetworkStackLatency(*this);
}

}  // namespace bedrock_protocol
