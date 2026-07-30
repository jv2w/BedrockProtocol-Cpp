/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeClimateData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeClimateData final {
public:
    BiomeClimateData(float temperature, float downfall, float snowAccumulationMin, float snowAccumulationMax)
        : temperature(temperature),
          downfall(downfall),
          snowAccumulationMin(snowAccumulationMin),
          snowAccumulationMax(snowAccumulationMax)
    {
    }

    [[nodiscard]] float getTemperature() const { return temperature; }

    [[nodiscard]] float getDownfall() const { return downfall; }

    [[nodiscard]] float getSnowAccumulationMin() const { return snowAccumulationMin; }

    [[nodiscard]] float getSnowAccumulationMax() const { return snowAccumulationMax; }

    /** @throws DataDecodeException */
    static BiomeClimateData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float temperature;
    float downfall;
    float snowAccumulationMin;
    float snowAccumulationMax;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
