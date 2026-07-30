/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPosition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPosition.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;

SubChunkPosition SubChunkPosition::readFixedInts(encoding::ByteBufferReader &in)
{
    const auto x = LE::readSignedInt(in);
    const auto y = LE::readSignedInt(in);
    const auto z = LE::readSignedInt(in);

    return SubChunkPosition(x, y, z);
}

SubChunkPosition SubChunkPosition::readVarInts(encoding::ByteBufferReader &in)
{
    const auto x = VarInt::readSignedInt(in);
    const auto y = VarInt::readSignedInt(in);
    const auto z = VarInt::readSignedInt(in);

    return SubChunkPosition(x, y, z);
}

void SubChunkPosition::writeFixedInts(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedInt(out, x);
    LE::writeSignedInt(out, y);
    LE::writeSignedInt(out, z);
}

void SubChunkPosition::writeVarInts(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, x);
    VarInt::writeSignedInt(out, y);
    VarInt::writeSignedInt(out, z);
}

}  // namespace bedrock_protocol::types
