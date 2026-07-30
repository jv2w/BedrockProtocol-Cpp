/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClockMarkerData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClockMarkerData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

SyncWorldClockMarkerData SyncWorldClockMarkerData::read(encoding::ByteBufferReader &in)
{
    const auto id = VarInt::readUnsignedLong(in);
    auto name = CommonTypes::getString(in);
    const auto time = VarInt::readSignedInt(in);
    auto period = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readSignedInt(in);
    });

    return SyncWorldClockMarkerData(id, std::move(name), time, period);
}

void SyncWorldClockMarkerData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, id);
    CommonTypes::putString(out, name);
    VarInt::writeSignedInt(out, time);
    CommonTypes::writeOptional(out, period, [](encoding::ByteBufferWriter &out, std::int32_t value) {
        LE::writeSignedInt(out, value);
    });
}

}  // namespace bedrock_protocol::types
