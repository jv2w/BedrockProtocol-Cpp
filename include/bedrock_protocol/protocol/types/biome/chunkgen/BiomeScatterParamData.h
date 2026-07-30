/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeScatterParamData.php
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
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeCoordinateData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeScatterParamData final {
public:
    BiomeScatterParamData(std::vector<BiomeCoordinateData> coordinates, std::int32_t evalOrder,
                          std::int32_t chancePercentType, std::int16_t chancePercent, std::int32_t chanceNumerator,
                          std::int32_t chanceDenominator, std::int32_t iterationsType, std::int16_t iterations)
        : coordinates(std::move(coordinates)),
          evalOrder(evalOrder),
          chancePercentType(chancePercentType),
          chancePercent(chancePercent),
          chanceNumerator(chanceNumerator),
          chanceDenominator(chanceDenominator),
          iterationsType(iterationsType),
          iterations(iterations)
    {
    }

    [[nodiscard]] const std::vector<BiomeCoordinateData> &getCoordinates() const { return coordinates; }

    [[nodiscard]] std::int32_t getEvalOrder() const { return evalOrder; }

    [[nodiscard]] std::int32_t getChancePercentType() const { return chancePercentType; }

    [[nodiscard]] std::int16_t getChancePercent() const { return chancePercent; }

    [[nodiscard]] std::int32_t getChanceNumerator() const { return chanceNumerator; }

    [[nodiscard]] std::int32_t getChanceDenominator() const { return chanceDenominator; }

    [[nodiscard]] std::int32_t getIterationsType() const { return iterationsType; }

    [[nodiscard]] std::int16_t getIterations() const { return iterations; }

    /** @throws DataDecodeException */
    static BiomeScatterParamData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<BiomeCoordinateData> coordinates;
    std::int32_t evalOrder;
    std::int32_t chancePercentType;
    std::int16_t chancePercent;
    std::int32_t chanceNumerator;
    std::int32_t chanceDenominator;
    std::int32_t iterationsType;
    std::int16_t iterations;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
