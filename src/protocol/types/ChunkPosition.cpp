/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ChunkPosition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ChunkPosition.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

ChunkPosition ChunkPosition::read(encoding::ByteBufferReader &in)
{
    const auto x = VarInt::readSignedInt(in);
    const auto z = VarInt::readSignedInt(in);

    return ChunkPosition(x, z);
}

void ChunkPosition::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, x);
    VarInt::writeSignedInt(out, z);
}

}  // namespace bedrock_protocol::types
