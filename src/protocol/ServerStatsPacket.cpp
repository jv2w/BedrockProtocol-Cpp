/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerStatsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerStatsPacket.h"

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

ServerStatsPacket ServerStatsPacket::create(float serverTime, float networkTime)
{
    ServerStatsPacket result;
    result.serverTime = serverTime;
    result.networkTime = networkTime;
    return result;
}

void ServerStatsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    serverTime = encoding::LE::readFloat(in);
    networkTime = encoding::LE::readFloat(in);

}

void ServerStatsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, serverTime);
    encoding::LE::writeFloat(out, networkTime);

}

bool ServerStatsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerStats(*this);
}

}  // namespace bedrock_protocol
