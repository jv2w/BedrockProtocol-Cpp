/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClocksInitializeRegistry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SyncWorldClocksInitializeRegistry.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

SyncWorldClocksInitializeRegistry SyncWorldClocksInitializeRegistry::read(encoding::ByteBufferReader &in)
{
    std::vector<SyncWorldClockData> clockData;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        clockData.push_back(SyncWorldClockData::read(in));
    }

    return SyncWorldClocksInitializeRegistry(std::move(clockData));
}

void SyncWorldClocksInitializeRegistry::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(clockData.size()));
    for (const auto &data : clockData) {
        data.write(out);
    }
}

}  // namespace bedrock_protocol::types
