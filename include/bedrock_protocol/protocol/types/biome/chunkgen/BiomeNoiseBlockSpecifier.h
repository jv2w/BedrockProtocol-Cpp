/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeNoiseBlockSpecifier.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeNoiseBlockSpecifier final {
public:
    BiomeNoiseBlockSpecifier(std::string noise, float threshold, float min, float max, std::uint32_t block)
        : noise(std::move(noise)), threshold(threshold), min(min), max(max), block(block)
    {
    }

    [[nodiscard]] const std::string &getNoise() const { return noise; }

    [[nodiscard]] float getThreshold() const { return threshold; }

    [[nodiscard]] float getMin() const { return min; }

    [[nodiscard]] float getMax() const { return max; }

    [[nodiscard]] std::uint32_t getBlock() const { return block; }

    /** @throws DataDecodeException */
    static BiomeNoiseBlockSpecifier read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string noise;
    float threshold;
    float min;
    float max;
    std::uint32_t block;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
