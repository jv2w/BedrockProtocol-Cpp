/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/BiomeDefinitionData.php
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

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeDefinitionChunkGenData.h"

namespace bedrock_protocol::types::biome {

class BiomeDefinitionData final {
public:
    BiomeDefinitionData(std::uint16_t nameIndex, std::uint16_t id, float temperature, float downfall, float foliageSnow,
                        float depth, float scale, color::Color mapWaterColor, bool rain,
                        std::optional<std::vector<std::uint16_t>> tagIndexes,
                        std::optional<chunkgen::BiomeDefinitionChunkGenData> chunkGenData = std::nullopt)
        : nameIndex(nameIndex),
          id(id),
          temperature(temperature),
          downfall(downfall),
          foliageSnow(foliageSnow),
          depth(depth),
          scale(scale),
          mapWaterColor(mapWaterColor),
          rain(rain),
          tagIndexes(std::move(tagIndexes)),
          chunkGenData(std::move(chunkGenData))
    {
    }

    [[nodiscard]] std::uint16_t getNameIndex() const { return nameIndex; }

    [[nodiscard]] std::uint16_t getId() const { return id; }

    [[nodiscard]] float getTemperature() const { return temperature; }

    [[nodiscard]] float getDownfall() const { return downfall; }

    [[nodiscard]] float getFoliageSnow() const { return foliageSnow; }

    [[nodiscard]] float getDepth() const { return depth; }

    [[nodiscard]] float getScale() const { return scale; }

    [[nodiscard]] color::Color getMapWaterColor() const { return mapWaterColor; }

    [[nodiscard]] bool hasRain() const { return rain; }

    [[nodiscard]] const std::optional<std::vector<std::uint16_t>> &getTagIndexes() const { return tagIndexes; }

    [[nodiscard]] const std::optional<chunkgen::BiomeDefinitionChunkGenData> &getChunkGenData() const
    {
        return chunkGenData;
    }

    /** @throws DataDecodeException */
    static BiomeDefinitionData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint16_t nameIndex;
    std::uint16_t id;
    float temperature;
    float downfall;
    float foliageSnow;
    float depth;
    float scale;
    color::Color mapWaterColor;
    bool rain;
    std::optional<std::vector<std::uint16_t>> tagIndexes;
    std::optional<chunkgen::BiomeDefinitionChunkGenData> chunkGenData;
};

}  // namespace bedrock_protocol::types::biome
