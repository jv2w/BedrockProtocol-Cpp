/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClockStateData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClockStateData.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

SyncWorldClockStateData SyncWorldClockStateData::read(encoding::ByteBufferReader &in)
{
    const auto clockId = VarInt::readUnsignedLong(in);
    const auto time = VarInt::readSignedInt(in);
    const auto paused = CommonTypes::getBool(in);

    return SyncWorldClockStateData(clockId, time, paused);
}

void SyncWorldClockStateData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, clockId);
    VarInt::writeSignedInt(out, time);
    CommonTypes::putBool(out, paused);
}

}  // namespace bedrock_protocol::types
