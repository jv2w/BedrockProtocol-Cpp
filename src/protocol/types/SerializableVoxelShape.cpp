/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SerializableVoxelShape.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SerializableVoxelShape.h"

#include <cstdint>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;

SerializableVoxelShape SerializableVoxelShape::read(encoding::ByteBufferReader &in)
{
    auto cells = SerializableVoxelCells::read(in);

    std::vector<float> xCoordinates;
    for (std::uint32_t i = 0, xCoordinatesCount = VarInt::readUnsignedInt(in); i < xCoordinatesCount; ++i) {
        xCoordinates.push_back(LE::readFloat(in));
    }

    std::vector<float> yCoordinates;
    for (std::uint32_t i = 0, yCoordinatesCount = VarInt::readUnsignedInt(in); i < yCoordinatesCount; ++i) {
        yCoordinates.push_back(LE::readFloat(in));
    }

    std::vector<float> zCoordinates;
    for (std::uint32_t i = 0, zCoordinatesCount = VarInt::readUnsignedInt(in); i < zCoordinatesCount; ++i) {
        zCoordinates.push_back(LE::readFloat(in));
    }

    return SerializableVoxelShape(
        std::move(cells),
        std::move(xCoordinates),
        std::move(yCoordinates),
        std::move(zCoordinates)
    );
}

void SerializableVoxelShape::write(encoding::ByteBufferWriter &out) const
{
    cells.write(out);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(xCoordinates.size()));
    for (const auto &value : xCoordinates) {
        LE::writeFloat(out, value);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(yCoordinates.size()));
    for (const auto &value : yCoordinates) {
        LE::writeFloat(out, value);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(zCoordinates.size()));
    for (const auto &value : zCoordinates) {
        LE::writeFloat(out, value);
    }
}

}  // namespace bedrock_protocol::types
