/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LabTablePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LabTablePacket.h"

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

LabTablePacket LabTablePacket::create(std::uint8_t actionType, types::BlockPosition blockPosition, std::uint8_t reactionType)
{
    LabTablePacket result;
    result.actionType = actionType;
    result.blockPosition = std::move(blockPosition);
    result.reactionType = reactionType;
    return result;
}

void LabTablePacket::decodePayload(encoding::ByteBufferReader &in)
{
    actionType = encoding::Byte::readUnsigned(in);
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    reactionType = encoding::Byte::readUnsigned(in);

}

void LabTablePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, actionType);
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    encoding::Byte::writeUnsigned(out, reactionType);

}

bool LabTablePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLabTable(*this);
}

}  // namespace bedrock_protocol
