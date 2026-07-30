/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SyncWorldClocksRemoveTimeMarker.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClocksRemoveTimeMarker.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

SyncWorldClocksRemoveTimeMarker SyncWorldClocksRemoveTimeMarker::read(encoding::ByteBufferReader &in)
{
    const auto clockId = VarInt::readUnsignedLong(in);

    std::vector<std::uint64_t> markerIds;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        markerIds.push_back(VarInt::readUnsignedLong(in));
    }

    return SyncWorldClocksRemoveTimeMarker(clockId, std::move(markerIds));
}

void SyncWorldClocksRemoveTimeMarker::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, clockId);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(markerIds.size()));
    for (const auto &markerId : markerIds) {
        VarInt::writeUnsignedLong(out, markerId);
    }
}

}  // namespace bedrock_protocol::types
