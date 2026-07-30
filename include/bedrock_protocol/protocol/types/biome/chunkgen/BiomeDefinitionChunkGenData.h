/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeDefinitionChunkGenData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeClimateData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeMountainParamsData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeReplacementData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceBuilderData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeDefinitionChunkGenData final {
public:
    BiomeDefinitionChunkGenData(std::optional<BiomeClimateData> climate,
                                std::optional<BiomeConsolidatedFeaturesData> consolidatedFeatures,
                                std::optional<BiomeMountainParamsData> mountainParams,
                                std::optional<BiomeSurfaceMaterialAdjustmentData> surfaceMaterialAdjustment,
                                std::optional<BiomeOverworldGenRulesData> overworldGenRules,
                                std::optional<BiomeMultinoiseGenRulesData> multinoiseGenRules,
                                std::optional<BiomeLegacyWorldGenRulesData> legacyWorldGenRules,
                                std::optional<std::vector<BiomeReplacementData>> replacementsData,
                                std::optional<std::uint8_t> villageType,
                                std::optional<BiomeSurfaceBuilderData> surfaceBuilderData,
                                std::optional<BiomeSurfaceBuilderData> subSurfaceBuilderData)
        : climate(std::move(climate)),
          consolidatedFeatures(std::move(consolidatedFeatures)),
          mountainParams(std::move(mountainParams)),
          surfaceMaterialAdjustment(std::move(surfaceMaterialAdjustment)),
          overworldGenRules(std::move(overworldGenRules)),
          multinoiseGenRules(std::move(multinoiseGenRules)),
          legacyWorldGenRules(std::move(legacyWorldGenRules)),
          replacementsData(std::move(replacementsData)),
          villageType(villageType),
          surfaceBuilderData(std::move(surfaceBuilderData)),
          subSurfaceBuilderData(std::move(subSurfaceBuilderData))
    {
    }

    [[nodiscard]] const std::optional<BiomeClimateData> &getClimate() const { return climate; }

    [[nodiscard]] const std::optional<BiomeConsolidatedFeaturesData> &getConsolidatedFeatures() const
    {
        return consolidatedFeatures;
    }

    [[nodiscard]] const std::optional<BiomeMountainParamsData> &getMountainParams() const { return mountainParams; }

    [[nodiscard]] const std::optional<BiomeSurfaceMaterialAdjustmentData> &getSurfaceMaterialAdjustment() const
    {
        return surfaceMaterialAdjustment;
    }

    [[nodiscard]] const std::optional<BiomeOverworldGenRulesData> &getOverworldGenRules() const
    {
        return overworldGenRules;
    }

    [[nodiscard]] const std::optional<BiomeMultinoiseGenRulesData> &getMultinoiseGenRules() const
    {
        return multinoiseGenRules;
    }

    [[nodiscard]] const std::optional<BiomeLegacyWorldGenRulesData> &getLegacyWorldGenRules() const
    {
        return legacyWorldGenRules;
    }

    [[nodiscard]] const std::optional<std::vector<BiomeReplacementData>> &getReplacementsData() const
    {
        return replacementsData;
    }

    [[nodiscard]] const std::optional<std::uint8_t> &getVillageType() const { return villageType; }

    [[nodiscard]] const std::optional<BiomeSurfaceBuilderData> &getSurfaceBuilderData() const
    {
        return surfaceBuilderData;
    }

    [[nodiscard]] const std::optional<BiomeSurfaceBuilderData> &getSubSurfaceBuilderData() const
    {
        return subSurfaceBuilderData;
    }

    /** @throws DataDecodeException */
    static BiomeDefinitionChunkGenData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<BiomeClimateData> climate;
    std::optional<BiomeConsolidatedFeaturesData> consolidatedFeatures;
    std::optional<BiomeMountainParamsData> mountainParams;
    std::optional<BiomeSurfaceMaterialAdjustmentData> surfaceMaterialAdjustment;
    std::optional<BiomeOverworldGenRulesData> overworldGenRules;
    std::optional<BiomeMultinoiseGenRulesData> multinoiseGenRules;
    std::optional<BiomeLegacyWorldGenRulesData> legacyWorldGenRules;
    std::optional<std::vector<BiomeReplacementData>> replacementsData;
    std::optional<std::uint8_t> villageType;
    std::optional<BiomeSurfaceBuilderData> surfaceBuilderData;
    std::optional<BiomeSurfaceBuilderData> subSurfaceBuilderData;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
