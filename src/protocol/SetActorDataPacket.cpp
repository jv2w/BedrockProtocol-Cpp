/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetActorDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetActorDataPacket.h"

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

SetActorDataPacket SetActorDataPacket::create(std::uint64_t actorRuntimeId, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, std::uint64_t tick)
{
    SetActorDataPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.metadata = std::move(metadata);
    result.syncedProperties = std::move(syncedProperties);
    result.tick = tick;
    return result;
}

void SetActorDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    metadata = serializer::CommonTypes::getEntityMetadata(in);
    syncedProperties = types::entity::PropertySyncData::read(in);
    tick = encoding::VarInt::readUnsignedLong(in);

}

void SetActorDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putEntityMetadata(out, metadata);
    syncedProperties.write(out);
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool SetActorDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetActorData(*this);
}

}  // namespace bedrock_protocol
