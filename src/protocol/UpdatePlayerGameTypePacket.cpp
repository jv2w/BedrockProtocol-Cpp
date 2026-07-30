/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdatePlayerGameTypePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdatePlayerGameTypePacket.h"

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

UpdatePlayerGameTypePacket UpdatePlayerGameTypePacket::create(std::int32_t gameMode, std::int64_t playerActorUniqueId, std::uint64_t tick)
{
    UpdatePlayerGameTypePacket result;
    result.gameMode = gameMode;
    result.playerActorUniqueId = playerActorUniqueId;
    result.tick = tick;
    return result;
}

void UpdatePlayerGameTypePacket::decodePayload(encoding::ByteBufferReader &in)
{
    gameMode = encoding::VarInt::readSignedInt(in);
    playerActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    tick = encoding::VarInt::readUnsignedLong(in);

}

void UpdatePlayerGameTypePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, gameMode);
    serializer::CommonTypes::putActorUniqueId(out, playerActorUniqueId);
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool UpdatePlayerGameTypePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdatePlayerGameType(*this);
}

}  // namespace bedrock_protocol
