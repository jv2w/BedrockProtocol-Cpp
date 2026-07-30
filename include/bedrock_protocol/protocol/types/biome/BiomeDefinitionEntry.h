/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/BiomeDefinitionEntry.php
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
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeDefinitionChunkGenData.h"

namespace bedrock_protocol::types::biome {

class BiomeDefinitionEntry final {
public:
    BiomeDefinitionEntry(std::string biomeName, std::uint16_t id, float temperature, float downfall, float foliageSnow,
                         float depth, float scale, color::Color mapWaterColor, bool rain,
                         std::optional<std::vector<std::string>> tags,
                         std::optional<chunkgen::BiomeDefinitionChunkGenData> chunkGenData = std::nullopt)
        : biomeName(std::move(biomeName)),
          id(id),
          temperature(temperature),
          downfall(downfall),
          foliageSnow(foliageSnow),
          depth(depth),
          scale(scale),
          mapWaterColor(mapWaterColor),
          rain(rain),
          tags(std::move(tags)),
          chunkGenData(std::move(chunkGenData))
    {
    }

    [[nodiscard]] const std::string &getBiomeName() const { return biomeName; }

    [[nodiscard]] std::uint16_t getId() const { return id; }

    [[nodiscard]] float getTemperature() const { return temperature; }

    [[nodiscard]] float getDownfall() const { return downfall; }

    [[nodiscard]] float getFoliageSnow() const { return foliageSnow; }

    [[nodiscard]] float getDepth() const { return depth; }

    [[nodiscard]] float getScale() const { return scale; }

    [[nodiscard]] color::Color getMapWaterColor() const { return mapWaterColor; }

    [[nodiscard]] bool hasRain() const { return rain; }

    [[nodiscard]] const std::optional<std::vector<std::string>> &getTags() const { return tags; }

    [[nodiscard]] const std::optional<chunkgen::BiomeDefinitionChunkGenData> &getChunkGenData() const
    {
        return chunkGenData;
    }

private:
    std::string biomeName;
    std::uint16_t id;
    float temperature;
    float downfall;
    float foliageSnow;
    float depth;
    float scale;
    color::Color mapWaterColor;
    bool rain;
    std::optional<std::vector<std::string>> tags;
    std::optional<chunkgen::BiomeDefinitionChunkGenData> chunkGenData;
};

}  // namespace bedrock_protocol::types::biome
