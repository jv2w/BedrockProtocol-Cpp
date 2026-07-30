/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateBlockPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateBlockPacket.h"

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

UpdateBlockPacket UpdateBlockPacket::create(types::BlockPosition blockPosition, std::uint32_t blockRuntimeId, std::uint32_t flags, std::uint32_t dataLayerId)
{
    UpdateBlockPacket result;
    result.blockPosition = std::move(blockPosition);
    result.blockRuntimeId = blockRuntimeId;
    result.flags = flags;
    result.dataLayerId = dataLayerId;
    return result;
}

void UpdateBlockPacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    blockRuntimeId = encoding::VarInt::readUnsignedInt(in);
    flags = encoding::VarInt::readUnsignedInt(in);
    dataLayerId = encoding::VarInt::readUnsignedInt(in);

}

void UpdateBlockPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    encoding::VarInt::writeUnsignedInt(out, blockRuntimeId);
    encoding::VarInt::writeUnsignedInt(out, flags);
    encoding::VarInt::writeUnsignedInt(out, dataLayerId);

}

bool UpdateBlockPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateBlock(*this);
}

}  // namespace bedrock_protocol
