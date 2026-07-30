/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AddPaintingPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddPaintingPacket.h"

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

AddPaintingPacket AddPaintingPacket::create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, math::Vector3 position, std::int32_t direction, std::string title)
{
    AddPaintingPacket result;
    result.actorUniqueId = actorUniqueId;
    result.actorRuntimeId = actorRuntimeId;
    result.position = std::move(position);
    result.direction = direction;
    result.title = std::move(title);
    return result;
}

void AddPaintingPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    position = serializer::CommonTypes::getVector3(in);
    direction = encoding::VarInt::readSignedInt(in);
    title = serializer::CommonTypes::getString(in);

}

void AddPaintingPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putVector3(out, position);
    encoding::VarInt::writeSignedInt(out, direction);
    serializer::CommonTypes::putString(out, title);

}

bool AddPaintingPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddPainting(*this);
}

}  // namespace bedrock_protocol
