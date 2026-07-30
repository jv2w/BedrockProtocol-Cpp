/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeCoordinateData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeCoordinateData final {
public:
    BiomeCoordinateData(std::int32_t minValueType, std::int16_t minValue, std::int32_t maxValueType,
                        std::int16_t maxValue, std::uint32_t gridOffset, std::uint32_t gridStepSize,
                        std::int32_t distribution)
        : minValueType(minValueType),
          minValue(minValue),
          maxValueType(maxValueType),
          maxValue(maxValue),
          gridOffset(gridOffset),
          gridStepSize(gridStepSize),
          distribution(distribution)
    {
    }

    [[nodiscard]] std::int32_t getMinValueType() const { return minValueType; }

    [[nodiscard]] std::int16_t getMinValue() const { return minValue; }

    [[nodiscard]] std::int32_t getMaxValueType() const { return maxValueType; }

    [[nodiscard]] std::int16_t getMaxValue() const { return maxValue; }

    [[nodiscard]] std::uint32_t getGridOffset() const { return gridOffset; }

    [[nodiscard]] std::uint32_t getGridStepSize() const { return gridStepSize; }

    [[nodiscard]] std::int32_t getDistribution() const { return distribution; }

    /** @throws DataDecodeException */
    static BiomeCoordinateData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t minValueType;
    std::int16_t minValue;
    std::int32_t maxValueType;
    std::int16_t maxValue;
    std::uint32_t gridOffset;
    std::uint32_t gridStepSize;
    std::int32_t distribution;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
