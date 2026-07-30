/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/DimensionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/DimensionData.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

DimensionData DimensionData::read(encoding::ByteBufferReader &in)
{
    const auto maxHeight = VarInt::readSignedInt(in);
    const auto minHeight = VarInt::readSignedInt(in);
    const auto generator = VarInt::readSignedInt(in);
    const auto dimensionType = VarInt::readSignedInt(in);

    return DimensionData(maxHeight, minHeight, generator, dimensionType);
}

void DimensionData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, maxHeight);
    VarInt::writeSignedInt(out, minHeight);
    VarInt::writeSignedInt(out, generator);
    VarInt::writeSignedInt(out, dimensionType);
}

}  // namespace bedrock_protocol::types
