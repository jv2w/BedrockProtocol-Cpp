/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ContainerSetDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ContainerSetDataPacket.h"

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

ContainerSetDataPacket ContainerSetDataPacket::create(std::uint8_t windowId, std::int32_t property, std::int32_t value)
{
    ContainerSetDataPacket result;
    result.windowId = windowId;
    result.property = property;
    result.value = value;
    return result;
}

void ContainerSetDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::Byte::readUnsigned(in);
    property = encoding::VarInt::readSignedInt(in);
    value = encoding::VarInt::readSignedInt(in);

}

void ContainerSetDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, windowId);
    encoding::VarInt::writeSignedInt(out, property);
    encoding::VarInt::writeSignedInt(out, value);

}

bool ContainerSetDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleContainerSetData(*this);
}

}  // namespace bedrock_protocol
