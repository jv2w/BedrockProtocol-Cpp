/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeReplacementData.php
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

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeReplacementData final {
public:
    BiomeReplacementData(std::int16_t biome, std::int32_t dimension, std::vector<std::int16_t> targetBiomes,
                         float amount, std::uint32_t replacementIndex)
        : biome(biome),
          dimension(dimension),
          targetBiomes(std::move(targetBiomes)),
          amount(amount),
          replacementIndex(replacementIndex)
    {
    }

    [[nodiscard]] std::int16_t getBiome() const { return biome; }

    [[nodiscard]] std::int32_t getDimension() const { return dimension; }

    [[nodiscard]] const std::vector<std::int16_t> &getTargetBiomes() const { return targetBiomes; }

    [[nodiscard]] float getAmount() const { return amount; }

    [[nodiscard]] std::uint32_t getReplacementIndex() const { return replacementIndex; }

    /** @throws DataDecodeException */
    static BiomeReplacementData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int16_t biome;
    std::int32_t dimension;
    std::vector<std::int16_t> targetBiomes;
    float amount;
    std::uint32_t replacementIndex;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
