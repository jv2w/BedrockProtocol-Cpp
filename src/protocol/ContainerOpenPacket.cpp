/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ContainerOpenPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ContainerOpenPacket.h"

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

ContainerOpenPacket ContainerOpenPacket::create(std::uint8_t windowId, std::uint8_t windowType, types::BlockPosition blockPosition, std::int64_t actorUniqueId)
{
    ContainerOpenPacket result;
    result.windowId = windowId;
    result.windowType = windowType;
    result.blockPosition = std::move(blockPosition);
    result.actorUniqueId = actorUniqueId;
    return result;
}

ContainerOpenPacket ContainerOpenPacket::blockInv(std::uint8_t windowId, std::uint8_t windowType, types::BlockPosition blockPosition)
{
    return create(windowId, windowType, blockPosition, -1);
}

ContainerOpenPacket ContainerOpenPacket::entityInv(std::uint8_t windowId, std::uint8_t windowType, std::int64_t actorUniqueId)
{
    return create(windowId, windowType, types::BlockPosition(0, 0, 0), actorUniqueId);
}

void ContainerOpenPacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::Byte::readUnsigned(in);
    windowType = encoding::Byte::readUnsigned(in);
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);

}

void ContainerOpenPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, windowId);
    encoding::Byte::writeUnsigned(out, windowType);
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);

}

bool ContainerOpenPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleContainerOpen(*this);
}

}  // namespace bedrock_protocol
