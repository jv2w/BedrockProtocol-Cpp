/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerHotbarPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerHotbarPacket.h"

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

PlayerHotbarPacket PlayerHotbarPacket::create(std::uint32_t selectedHotbarSlot, std::uint8_t windowId, bool selectHotbarSlot)
{
    PlayerHotbarPacket result;
    result.selectedHotbarSlot = selectedHotbarSlot;
    result.windowId = windowId;
    result.selectHotbarSlot = selectHotbarSlot;
    return result;
}

void PlayerHotbarPacket::decodePayload(encoding::ByteBufferReader &in)
{
    selectedHotbarSlot = encoding::VarInt::readUnsignedInt(in);
    windowId = encoding::Byte::readUnsigned(in);
    selectHotbarSlot = serializer::CommonTypes::getBool(in);

}

void PlayerHotbarPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, selectedHotbarSlot);
    encoding::Byte::writeUnsigned(out, windowId);
    serializer::CommonTypes::putBool(out, selectHotbarSlot);

}

bool PlayerHotbarPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerHotbar(*this);
}

}  // namespace bedrock_protocol
