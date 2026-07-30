/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeConsolidatedFeaturesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeConsolidatedFeaturesData BiomeConsolidatedFeaturesData::read(encoding::ByteBufferReader &in)
{
    std::vector<BiomeConsolidatedFeatureData> features;

    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        features.push_back(BiomeConsolidatedFeatureData::read(in));
    }

    return BiomeConsolidatedFeaturesData(std::move(features));
}

void BiomeConsolidatedFeaturesData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(features.size()));
    for (const auto &feature : features) {
        feature.write(out);
    }
}

}  // namespace bedrock_protocol::types::biome::chunkgen
