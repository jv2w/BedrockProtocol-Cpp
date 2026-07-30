/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeConditionalTransformationData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeWeightedData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeConditionalTransformationData final {
public:
    BiomeConditionalTransformationData(std::vector<BiomeWeightedData> weightedBiomes, std::int16_t conditionJSON,
                                       std::uint32_t minPassingNeighbors)
        : weightedBiomes(std::move(weightedBiomes)),
          conditionJSON(conditionJSON),
          minPassingNeighbors(minPassingNeighbors)
    {
    }

    [[nodiscard]] const std::vector<BiomeWeightedData> &getWeightedBiomes() const { return weightedBiomes; }

    [[nodiscard]] std::int16_t getConditionJSON() const { return conditionJSON; }

    [[nodiscard]] std::uint32_t getMinPassingNeighbors() const { return minPassingNeighbors; }

    /** @throws DataDecodeException */
    static BiomeConditionalTransformationData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<BiomeWeightedData> weightedBiomes;
    std::int16_t conditionJSON;
    std::uint32_t minPassingNeighbors;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
