/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ArmorSlotAndDamagePair.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ArmorSlotAndDamagePair.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;

ArmorSlotAndDamagePair ArmorSlotAndDamagePair::read(encoding::ByteBufferReader &in)
{
    const auto slot = ArmorSlotFromPacket(Byte::readUnsigned(in));
    const auto damage = LE::readUnsignedShort(in);

    return ArmorSlotAndDamagePair(slot, damage);
}

void ArmorSlotAndDamagePair::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, static_cast<std::uint8_t>(slot));
    LE::writeUnsignedShort(out, damage);
}

}  // namespace bedrock_protocol::types
