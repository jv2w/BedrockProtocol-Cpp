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

namespace bedrock_protocol::types {

using encoding::LE;

/**
 * gophertunnel v1.58.0 minecraft/protocol/writer.go:215-218 and reader.go:278-281 write this as a fixed
 * 4-byte LE uint32. The StartGame-only varint form went away with GameRuleLegacy, so both StartGame and
 * GameRulesChanged now use this one encoding.
 */
void IntGameRule::encode(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, value);
}

IntGameRule IntGameRule::decode(encoding::ByteBufferReader &in, const bool isPlayerModifiable)
{
    return IntGameRule(LE::readUnsignedInt(in), isPlayerModifiable);
}

}  // namespace bedrock_protocol::types
