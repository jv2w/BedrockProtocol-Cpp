/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryWithoutCache.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPacketEntryWithoutCache.h"

namespace bedrock_protocol::types {

SubChunkPacketEntryWithoutCache SubChunkPacketEntryWithoutCache::read(encoding::ByteBufferReader &in)
{
    return SubChunkPacketEntryWithoutCache(SubChunkPacketEntryCommon::read(in, false));
}

void SubChunkPacketEntryWithoutCache::write(encoding::ByteBufferWriter &out) const
{
    base.write(out, false);
}

}  // namespace bedrock_protocol::types
