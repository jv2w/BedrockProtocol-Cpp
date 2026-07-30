/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeNoiseGradientSurfaceData final {
public:
    BiomeNoiseGradientSurfaceData(std::vector<std::uint32_t> nonReplaceableBlocks,
                                  std::vector<BiomeNoiseBlockSpecifier> gradientBlocks, std::string noiseSeed,
                                  std::uint32_t firstOctave, std::vector<float> amplitudes)
        : nonReplaceableBlocks(std::move(nonReplaceableBlocks)),
          gradientBlocks(std::move(gradientBlocks)),
          noiseSeed(std::move(noiseSeed)),
          firstOctave(firstOctave),
          amplitudes(std::move(amplitudes))
    {
    }

    [[nodiscard]] const std::vector<std::uint32_t> &getNonReplaceableBlocks() const { return nonReplaceableBlocks; }

    [[nodiscard]] const std::vector<BiomeNoiseBlockSpecifier> &getGradientBlocks() const { return gradientBlocks; }

    [[nodiscard]] const std::string &getNoiseSeed() const { return noiseSeed; }

    [[nodiscard]] std::uint32_t getFirstOctave() const { return firstOctave; }

    [[nodiscard]] const std::vector<float> &getAmplitudes() const { return amplitudes; }

    /** @throws DataDecodeException */
    static BiomeNoiseGradientSurfaceData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<std::uint32_t> nonReplaceableBlocks;
    std::vector<BiomeNoiseBlockSpecifier> gradientBlocks;
    std::string noiseSeed;
    std::uint32_t firstOctave;
    std::vector<float> amplitudes;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
