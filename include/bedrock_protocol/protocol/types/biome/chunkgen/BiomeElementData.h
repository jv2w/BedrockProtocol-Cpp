/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeElementData.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeElementData final {
public:
    BiomeElementData(float noiseFrequencyScale, float noiseLowerBound, float noiseUpperBound,
                     std::int32_t heightMinType, std::int16_t heightMin, std::int32_t heightMaxType,
                     std::int16_t heightMax, BiomeSurfaceMaterialData surfaceMaterial)
        : noiseFrequencyScale(noiseFrequencyScale),
          noiseLowerBound(noiseLowerBound),
          noiseUpperBound(noiseUpperBound),
          heightMinType(heightMinType),
          heightMin(heightMin),
          heightMaxType(heightMaxType),
          heightMax(heightMax),
          surfaceMaterial(std::move(surfaceMaterial))
    {
    }

    [[nodiscard]] float getNoiseFrequencyScale() const { return noiseFrequencyScale; }

    [[nodiscard]] float getNoiseLowerBound() const { return noiseLowerBound; }

    [[nodiscard]] float getNoiseUpperBound() const { return noiseUpperBound; }

    [[nodiscard]] std::int32_t getHeightMinType() const { return heightMinType; }

    [[nodiscard]] std::int16_t getHeightMin() const { return heightMin; }

    [[nodiscard]] std::int32_t getHeightMaxType() const { return heightMaxType; }

    [[nodiscard]] std::int16_t getHeightMax() const { return heightMax; }

    [[nodiscard]] const BiomeSurfaceMaterialData &getSurfaceMaterial() const { return surfaceMaterial; }

    /** @throws DataDecodeException */
    static BiomeElementData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float noiseFrequencyScale;
    float noiseLowerBound;
    float noiseUpperBound;
    std::int32_t heightMinType;
    std::int16_t heightMin;
    std::int32_t heightMaxType;
    std::int16_t heightMax;
    BiomeSurfaceMaterialData surfaceMaterial;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
