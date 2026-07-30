/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MapCreateLockedCopyPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MapCreateLockedCopyPacket.h"

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

MapCreateLockedCopyPacket MapCreateLockedCopyPacket::create(std::int64_t originalMapId, std::int64_t newMapId)
{
    MapCreateLockedCopyPacket result;
    result.originalMapId = originalMapId;
    result.newMapId = newMapId;
    return result;
}

void MapCreateLockedCopyPacket::decodePayload(encoding::ByteBufferReader &in)
{
    originalMapId = serializer::CommonTypes::getActorUniqueId(in);
    newMapId = serializer::CommonTypes::getActorUniqueId(in);

}

void MapCreateLockedCopyPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, originalMapId);
    serializer::CommonTypes::putActorUniqueId(out, newMapId);

}

bool MapCreateLockedCopyPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMapCreateLockedCopy(*this);
}

}  // namespace bedrock_protocol
