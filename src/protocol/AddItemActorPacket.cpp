/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AddItemActorPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddItemActorPacket.h"

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

AddItemActorPacket AddItemActorPacket::create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, types::inventory::ItemStackWrapper item, math::Vector3 position, std::optional<math::Vector3> motion, serializer::CommonTypes::EntityMetadata metadata, bool isFromFishing)
{
    AddItemActorPacket result;
    result.actorUniqueId = actorUniqueId;
    result.actorRuntimeId = actorRuntimeId;
    result.item = std::move(item);
    result.position = std::move(position);
    result.motion = std::move(motion);
    result.metadata = std::move(metadata);
    result.isFromFishing = isFromFishing;
    return result;
}

void AddItemActorPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    item = serializer::CommonTypes::getItemStackWrapper(in);
    position = serializer::CommonTypes::getVector3(in);
    motion = serializer::CommonTypes::getVector3(in);
    metadata = serializer::CommonTypes::getEntityMetadata(in);
    isFromFishing = serializer::CommonTypes::getBool(in);

}

void AddItemActorPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    //PHP throws Error when encoding a packet whose typed $item was never initialised; dereferencing an
    //empty optional would instead be undefined behaviour, so use value() to keep the PHP failure mode.
    serializer::CommonTypes::putItemStackWrapper(out, item.value());
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putVector3Nullable(out, motion);
    serializer::CommonTypes::putEntityMetadata(out, metadata);
    serializer::CommonTypes::putBool(out, isFromFishing);

}

bool AddItemActorPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddItemActor(*this);
}

}  // namespace bedrock_protocol
