/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ActorPickRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ActorPickRequestPacket.h"

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

ActorPickRequestPacket ActorPickRequestPacket::create(std::int64_t actorUniqueId, std::uint8_t hotbarSlot, bool addUserData)
{
    ActorPickRequestPacket result;
    result.actorUniqueId = actorUniqueId;
    result.hotbarSlot = hotbarSlot;
    result.addUserData = addUserData;
    return result;
}

void ActorPickRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = encoding::LE::readSignedLong(in);
    hotbarSlot = encoding::Byte::readUnsigned(in);
    addUserData = serializer::CommonTypes::getBool(in);

}

void ActorPickRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedLong(out, actorUniqueId);
    encoding::Byte::writeUnsigned(out, hotbarSlot);
    serializer::CommonTypes::putBool(out, addUserData);

}

bool ActorPickRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleActorPickRequest(*this);
}

}  // namespace bedrock_protocol
