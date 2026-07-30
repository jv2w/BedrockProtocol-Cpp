/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeConditionalTransformationData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeConditionalTransformationData BiomeConditionalTransformationData::read(encoding::ByteBufferReader &in)
{
    std::vector<BiomeWeightedData> weightedBiomes;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        weightedBiomes.push_back(BiomeWeightedData::read(in));
    }

    const auto conditionJSON = encoding::LE::readSignedShort(in);
    const auto minPassingNeighbors = encoding::LE::readUnsignedInt(in);

    return BiomeConditionalTransformationData(std::move(weightedBiomes), conditionJSON, minPassingNeighbors);
}

void BiomeConditionalTransformationData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(weightedBiomes.size()));
    for (const auto &biome : weightedBiomes) {
        biome.write(out);
    }

    encoding::LE::writeSignedShort(out, conditionJSON);
    encoding::LE::writeUnsignedInt(out, minPassingNeighbors);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
