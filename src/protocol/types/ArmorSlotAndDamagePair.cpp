/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ArmorSlotAndDamagePair.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ArmorSlotAndDamagePair.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;

ArmorSlotAndDamagePair ArmorSlotAndDamagePair::read(encoding::ByteBufferReader &in)
{
    //The slot is a signed varint, not a byte: Endstone r26_u4 ArmorSlotAndDamagePair.json gives
    //varint32, Mojang's json marks it int32 + Compression, and gophertunnel v1.58.0
    //minecraft/protocol/player.go:175 writes Varint32.
    const auto slot = ArmorSlotFromPacket(VarInt::readSignedInt(in));
    const auto damage = LE::readUnsignedShort(in);

    return ArmorSlotAndDamagePair(slot, damage);
}

void ArmorSlotAndDamagePair::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, static_cast<std::int32_t>(slot));
    LE::writeUnsignedShort(out, damage);
}

}  // namespace bedrock_protocol::types
