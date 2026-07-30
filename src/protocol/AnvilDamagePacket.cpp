/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AnvilDamagePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AnvilDamagePacket.h"

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

AnvilDamagePacket AnvilDamagePacket::create(types::BlockPosition blockPosition, std::uint8_t damageAmount)
{
    AnvilDamagePacket result;
    result.blockPosition = std::move(blockPosition);
    result.damageAmount = damageAmount;
    return result;
}

void AnvilDamagePacket::decodePayload(encoding::ByteBufferReader &in)
{
    damageAmount = encoding::Byte::readUnsigned(in);
    blockPosition = serializer::CommonTypes::getBlockPosition(in);

}

void AnvilDamagePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, damageAmount);
    serializer::CommonTypes::putBlockPosition(out, blockPosition);

}

bool AnvilDamagePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAnvilDamage(*this);
}

}  // namespace bedrock_protocol
