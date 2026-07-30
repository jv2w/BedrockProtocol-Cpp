/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetHealthPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetHealthPacket.h"

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

SetHealthPacket SetHealthPacket::create(std::int32_t health)
{
    SetHealthPacket result;
    result.health = health;
    return result;
}

void SetHealthPacket::decodePayload(encoding::ByteBufferReader &in)
{
    health = encoding::VarInt::readSignedInt(in);

}

void SetHealthPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, health);

}

bool SetHealthPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetHealth(*this);
}

}  // namespace bedrock_protocol
