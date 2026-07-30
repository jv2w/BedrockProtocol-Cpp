/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CompletedUsingItemPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CompletedUsingItemPacket.h"

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

CompletedUsingItemPacket CompletedUsingItemPacket::create(std::int16_t itemId, std::int32_t action)
{
    CompletedUsingItemPacket result;
    result.itemId = itemId;
    result.action = action;
    return result;
}

void CompletedUsingItemPacket::decodePayload(encoding::ByteBufferReader &in)
{
    itemId = encoding::LE::readSignedShort(in);
    action = encoding::LE::readSignedInt(in);

}

void CompletedUsingItemPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedShort(out, itemId);
    encoding::LE::writeSignedInt(out, action);

}

bool CompletedUsingItemPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCompletedUsingItem(*this);
}

}  // namespace bedrock_protocol
