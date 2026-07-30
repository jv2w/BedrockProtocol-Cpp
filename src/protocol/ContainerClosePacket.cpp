/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ContainerClosePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ContainerClosePacket.h"

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

ContainerClosePacket ContainerClosePacket::create(std::uint8_t windowId, std::uint8_t windowType, bool server)
{
    ContainerClosePacket result;
    result.windowId = windowId;
    result.windowType = windowType;
    result.server = server;
    return result;
}

void ContainerClosePacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::Byte::readUnsigned(in);
    windowType = encoding::Byte::readUnsigned(in);
    server = serializer::CommonTypes::getBool(in);

}

void ContainerClosePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, windowId);
    encoding::Byte::writeUnsigned(out, windowType);
    serializer::CommonTypes::putBool(out, server);

}

bool ContainerClosePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleContainerClose(*this);
}

}  // namespace bedrock_protocol
