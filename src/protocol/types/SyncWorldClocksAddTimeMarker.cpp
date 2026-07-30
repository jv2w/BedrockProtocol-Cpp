/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClocksAddTimeMarker.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClocksAddTimeMarker.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

SyncWorldClocksAddTimeMarker SyncWorldClocksAddTimeMarker::read(encoding::ByteBufferReader &in)
{
    const auto clockId = VarInt::readUnsignedLong(in);

    std::vector<SyncWorldClockMarkerData> markers;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        markers.push_back(SyncWorldClockMarkerData::read(in));
    }

    return SyncWorldClocksAddTimeMarker(clockId, std::move(markers));
}

void SyncWorldClocksAddTimeMarker::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, clockId);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(markers.size()));
    for (const auto &marker : markers) {
        marker.write(out);
    }
}

}  // namespace bedrock_protocol::types
