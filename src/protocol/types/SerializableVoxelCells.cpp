/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SerializableVoxelCells.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SerializableVoxelCells.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::VarInt;

SerializableVoxelCells SerializableVoxelCells::read(encoding::ByteBufferReader &in)
{
    const auto xSize = Byte::readUnsigned(in);
    const auto ySize = Byte::readUnsigned(in);
    const auto zSize = Byte::readUnsigned(in);

    std::vector<std::uint8_t> storage;
    for (std::uint32_t i = 0, storageCount = VarInt::readUnsignedInt(in); i < storageCount; ++i) {
        storage.push_back(Byte::readUnsigned(in));
    }

    return SerializableVoxelCells(
        xSize,
        ySize,
        zSize,
        std::move(storage)
    );
}

void SerializableVoxelCells::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, xSize);
    Byte::writeUnsigned(out, ySize);
    Byte::writeUnsigned(out, zSize);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(storage.size()));
    for (const auto &value : storage) {
        Byte::writeUnsigned(out, value);
    }
}

}  // namespace bedrock_protocol::types
