/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BlockPickRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BlockPickRequestPacket.h"

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

BlockPickRequestPacket BlockPickRequestPacket::create(types::BlockPosition blockPosition, bool addUserData, std::uint8_t hotbarSlot)
{
    BlockPickRequestPacket result;
    result.blockPosition = std::move(blockPosition);
    result.addUserData = addUserData;
    result.hotbarSlot = hotbarSlot;
    return result;
}

void BlockPickRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    addUserData = serializer::CommonTypes::getBool(in);
    hotbarSlot = encoding::Byte::readUnsigned(in);

}

void BlockPickRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    serializer::CommonTypes::putBool(out, addUserData);
    encoding::Byte::writeUnsigned(out, hotbarSlot);

}

bool BlockPickRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBlockPickRequest(*this);
}

}  // namespace bedrock_protocol
