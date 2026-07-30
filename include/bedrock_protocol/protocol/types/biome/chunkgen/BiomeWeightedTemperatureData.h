/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeWeightedTemperatureData.php
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

class BiomeWeightedTemperatureData final {
public:
    BiomeWeightedTemperatureData(std::int32_t temperature, std::uint32_t weight)
        : temperature(temperature), weight(weight)
    {
    }

    [[nodiscard]] std::int32_t getTemperature() const { return temperature; }

    [[nodiscard]] std::uint32_t getWeight() const { return weight; }

    /** @throws DataDecodeException */
    static BiomeWeightedTemperatureData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t temperature;
    std::uint32_t weight;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
