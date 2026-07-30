/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeWeightedData.php
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

class BiomeWeightedData final {
public:
    BiomeWeightedData(std::int16_t biome, std::uint32_t weight) : biome(biome), weight(weight) {}

    [[nodiscard]] std::int16_t getBiome() const { return biome; }

    [[nodiscard]] std::uint32_t getWeight() const { return weight; }

    /** @throws DataDecodeException */
    static BiomeWeightedData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int16_t biome;
    std::uint32_t weight;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
