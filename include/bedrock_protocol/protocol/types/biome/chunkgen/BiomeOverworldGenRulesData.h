/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeOverworldGenRulesData.php
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
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeWeightedData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeOverworldGenRulesData final {
public:
    BiomeOverworldGenRulesData(std::vector<BiomeWeightedData> hillTransformations,
                               std::vector<BiomeWeightedData> mutateTransformations,
                               std::vector<BiomeWeightedData> riverTransformations,
                               std::vector<BiomeWeightedData> shoreTransformations,
                               std::vector<BiomeConditionalTransformationData> preHillsEdges,
                               std::vector<BiomeConditionalTransformationData> postShoreEdges,
                               std::vector<BiomeWeightedTemperatureData> climates)
        : hillTransformations(std::move(hillTransformations)),
          mutateTransformations(std::move(mutateTransformations)),
          riverTransformations(std::move(riverTransformations)),
          shoreTransformations(std::move(shoreTransformations)),
          preHillsEdges(std::move(preHillsEdges)),
          postShoreEdges(std::move(postShoreEdges)),
          climates(std::move(climates))
    {
    }

    [[nodiscard]] const std::vector<BiomeWeightedData> &getHillTransformations() const { return hillTransformations; }

    [[nodiscard]] const std::vector<BiomeWeightedData> &getMutateTransformations() const
    {
        return mutateTransformations;
    }

    [[nodiscard]] const std::vector<BiomeWeightedData> &getRiverTransformations() const
    {
        return riverTransformations;
    }

    [[nodiscard]] const std::vector<BiomeWeightedData> &getShoreTransformations() const
    {
        return shoreTransformations;
    }

    [[nodiscard]] const std::vector<BiomeConditionalTransformationData> &getPreHillsEdges() const
    {
        return preHillsEdges;
    }

    [[nodiscard]] const std::vector<BiomeConditionalTransformationData> &getPostShoreEdges() const
    {
        return postShoreEdges;
    }

    [[nodiscard]] const std::vector<BiomeWeightedTemperatureData> &getClimates() const { return climates; }

    /** @throws DataDecodeException */
    static BiomeOverworldGenRulesData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<BiomeWeightedData> hillTransformations;
    std::vector<BiomeWeightedData> mutateTransformations;
    std::vector<BiomeWeightedData> riverTransformations;
    std::vector<BiomeWeightedData> shoreTransformations;
    std::vector<BiomeConditionalTransformationData> preHillsEdges;
    std::vector<BiomeConditionalTransformationData> postShoreEdges;
    std::vector<BiomeWeightedTemperatureData> climates;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
