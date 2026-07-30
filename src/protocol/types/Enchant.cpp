/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/Enchant.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/Enchant.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::VarInt;

Enchant Enchant::read(encoding::ByteBufferReader &in)
{
    const auto id = VarInt::readUnsignedInt(in);
    const auto level = Byte::readUnsigned(in);
    return Enchant(id, level);
}

void Enchant::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, id);
    Byte::writeUnsigned(out, level);
}

}  // namespace bedrock_protocol::types
