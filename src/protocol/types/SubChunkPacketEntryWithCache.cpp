/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryWithCache.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPacketEntryWithCache.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::LE;

SubChunkPacketEntryWithCache SubChunkPacketEntryWithCache::read(encoding::ByteBufferReader &in)
{
    auto base = SubChunkPacketEntryCommon::read(in, true);
    const auto usedBlobHash = LE::readUnsignedLong(in);

    return SubChunkPacketEntryWithCache(std::move(base), usedBlobHash);
}

void SubChunkPacketEntryWithCache::write(encoding::ByteBufferWriter &out) const
{
    base.write(out, true);
    LE::writeUnsignedLong(out, usedBlobHash);
}

}  // namespace bedrock_protocol::types
