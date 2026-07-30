/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetSpawnPositionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetSpawnPositionPacket.h"

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

SetSpawnPositionPacket SetSpawnPositionPacket::create(std::int32_t spawnType, types::BlockPosition spawnPosition, std::int32_t dimension, types::BlockPosition causingBlockPosition)
{
    SetSpawnPositionPacket result;
    result.spawnType = spawnType;
    result.spawnPosition = std::move(spawnPosition);
    result.dimension = dimension;
    result.causingBlockPosition = std::move(causingBlockPosition);
    return result;
}

SetSpawnPositionPacket SetSpawnPositionPacket::playerSpawn(types::BlockPosition spawnPosition, std::int32_t dimension, types::BlockPosition causingBlockPosition)
{
    return create(TYPE_PLAYER_SPAWN, spawnPosition, dimension, causingBlockPosition);
}

SetSpawnPositionPacket SetSpawnPositionPacket::worldSpawn(types::BlockPosition spawnPosition, std::int32_t dimension)
{
    return create(TYPE_WORLD_SPAWN, spawnPosition, dimension, types::BlockPosition(INT32_MIN, INT32_MIN, INT32_MIN));
}

void SetSpawnPositionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    spawnType = encoding::VarInt::readSignedInt(in);
    spawnPosition = serializer::CommonTypes::getBlockPosition(in);
    dimension = encoding::VarInt::readSignedInt(in);
    causingBlockPosition = serializer::CommonTypes::getBlockPosition(in);

}

void SetSpawnPositionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, spawnType);
    serializer::CommonTypes::putBlockPosition(out, spawnPosition);
    encoding::VarInt::writeSignedInt(out, dimension);
    serializer::CommonTypes::putBlockPosition(out, causingBlockPosition);

}

bool SetSpawnPositionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetSpawnPosition(*this);
}

}  // namespace bedrock_protocol
