/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeSurfaceMaterialAdjustmentData BiomeSurfaceMaterialAdjustmentData::read(encoding::ByteBufferReader &in)
{
    std::vector<BiomeElementData> adjustments;

    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        adjustments.push_back(BiomeElementData::read(in));
    }

    return BiomeSurfaceMaterialAdjustmentData(std::move(adjustments));
}

void BiomeSurfaceMaterialAdjustmentData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(adjustments.size()));
    for (const auto &adjustment : adjustments) {
        adjustment.write(out);
    }
}

}  // namespace bedrock_protocol::types::biome::chunkgen
