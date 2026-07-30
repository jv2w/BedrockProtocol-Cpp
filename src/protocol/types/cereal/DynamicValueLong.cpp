/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/cereal/DynamicValueLong.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/cereal/DynamicValueLong.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::cereal {

using encoding::LE;

DynamicValueLong DynamicValueLong::readValue(encoding::ByteBufferReader &in)
{
    return DynamicValueLong(LE::readSignedLong(in));
}

void DynamicValueLong::writeValue(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedLong(out, value);
}

}  // namespace bedrock_protocol::types::cereal
