/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/IntGameRule.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/IntGameRule.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;

void IntGameRule::encode(encoding::ByteBufferWriter &out, const bool isStartGame) const
{
    if (isStartGame) {
        VarInt::writeUnsignedInt(out, value);
    }
    else {
        LE::writeUnsignedInt(out, value);
    }
}

IntGameRule IntGameRule::decode(encoding::ByteBufferReader &in, const bool isPlayerModifiable, const bool isStartGame)
{
    return IntGameRule(isStartGame ? VarInt::readUnsignedInt(in) : LE::readUnsignedInt(in), isPlayerModifiable);
}

}  // namespace bedrock_protocol::types
