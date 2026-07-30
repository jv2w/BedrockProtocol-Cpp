/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/HurtArmorPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/HurtArmorPacket.h"

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

HurtArmorPacket HurtArmorPacket::create(std::int32_t cause, std::int32_t health, std::uint64_t armorSlotFlags)
{
    HurtArmorPacket result;
    result.cause = cause;
    result.health = health;
    result.armorSlotFlags = armorSlotFlags;
    return result;
}

void HurtArmorPacket::decodePayload(encoding::ByteBufferReader &in)
{
    cause = encoding::VarInt::readSignedInt(in);
    health = encoding::VarInt::readSignedInt(in);
    armorSlotFlags = encoding::VarInt::readUnsignedLong(in);

}

void HurtArmorPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, cause);
    encoding::VarInt::writeSignedInt(out, health);
    encoding::VarInt::writeUnsignedLong(out, armorSlotFlags);

}

bool HurtArmorPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleHurtArmor(*this);
}

}  // namespace bedrock_protocol
