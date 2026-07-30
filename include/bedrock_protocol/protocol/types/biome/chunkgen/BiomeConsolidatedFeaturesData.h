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

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeConsolidatedFeaturesData final {
public:
    explicit BiomeConsolidatedFeaturesData(std::vector<BiomeConsolidatedFeatureData> features)
        : features(std::move(features))
    {
    }

    [[nodiscard]] const std::vector<BiomeConsolidatedFeatureData> &getFeatures() const { return features; }

    /** @throws DataDecodeException */
    static BiomeConsolidatedFeaturesData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<BiomeConsolidatedFeatureData> features;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
