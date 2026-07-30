/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeCoordinateData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeCoordinateData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeCoordinateData BiomeCoordinateData::read(encoding::ByteBufferReader &in)
{
    const auto minValueType = encoding::VarInt::readSignedInt(in);
    const auto minValue = encoding::LE::readSignedShort(in);
    const auto maxValueType = encoding::VarInt::readSignedInt(in);
    const auto maxValue = encoding::LE::readSignedShort(in);
    const auto gridOffset = encoding::LE::readUnsignedInt(in);
    const auto gridStepSize = encoding::LE::readUnsignedInt(in);
    const auto distribution = encoding::VarInt::readSignedInt(in);

    return BiomeCoordinateData(minValueType, minValue, maxValueType, maxValue, gridOffset, gridStepSize, distribution);
}

void BiomeCoordinateData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, minValueType);
    encoding::LE::writeSignedShort(out, minValue);
    encoding::VarInt::writeSignedInt(out, maxValueType);
    encoding::LE::writeSignedShort(out, maxValue);
    encoding::LE::writeUnsignedInt(out, gridOffset);
    encoding::LE::writeUnsignedInt(out, gridStepSize);
    encoding::VarInt::writeSignedInt(out, distribution);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
