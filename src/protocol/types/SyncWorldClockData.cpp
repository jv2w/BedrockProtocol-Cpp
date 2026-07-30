/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SyncWorldClockData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClockData.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

SyncWorldClockData SyncWorldClockData::read(encoding::ByteBufferReader &in)
{
    const auto id = VarInt::readUnsignedLong(in);
    auto name = CommonTypes::getString(in);
    const auto time = VarInt::readSignedInt(in);
    const auto paused = CommonTypes::getBool(in);

    std::vector<SyncWorldClockMarkerData> markers;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        markers.push_back(SyncWorldClockMarkerData::read(in));
    }

    return SyncWorldClockData(
        id,
        std::move(name),
        time,
        paused,
        std::move(markers)
    );
}

void SyncWorldClockData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, id);
    CommonTypes::putString(out, name);
    VarInt::writeSignedInt(out, time);
    CommonTypes::putBool(out, paused);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(markers.size()));
    for (const auto &marker : markers) {
        marker.write(out);
    }
}

}  // namespace bedrock_protocol::types
