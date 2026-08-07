/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/BoolGameRule.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/BoolGameRule.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

void BoolGameRule::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBool(out, value);
}

BoolGameRule BoolGameRule::decode(encoding::ByteBufferReader &in, bool isPlayerModifiable)
{
    return BoolGameRule(CommonTypes::getBool(in), isPlayerModifiable);
}

}  // namespace bedrock_protocol::types
