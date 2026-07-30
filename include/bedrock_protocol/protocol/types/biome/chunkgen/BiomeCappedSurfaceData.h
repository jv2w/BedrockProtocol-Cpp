/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeCappedSurfaceData.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeCappedSurfaceData final {
public:
    BiomeCappedSurfaceData(std::vector<std::uint32_t> floorBlocks, std::vector<std::uint32_t> ceilingBlocks,
                           std::optional<std::uint32_t> seaBlock, std::optional<std::uint32_t> foundationBlock,
                           std::optional<std::uint32_t> beachBlock)
        : floorBlocks(std::move(floorBlocks)),
          ceilingBlocks(std::move(ceilingBlocks)),
          seaBlock(seaBlock),
          foundationBlock(foundationBlock),
          beachBlock(beachBlock)
    {
    }

    [[nodiscard]] const std::vector<std::uint32_t> &getFloorBlocks() const { return floorBlocks; }

    [[nodiscard]] const std::vector<std::uint32_t> &getCeilingBlocks() const { return ceilingBlocks; }

    [[nodiscard]] const std::optional<std::uint32_t> &getSeaBlock() const { return seaBlock; }

    [[nodiscard]] const std::optional<std::uint32_t> &getFoundationBlock() const { return foundationBlock; }

    [[nodiscard]] const std::optional<std::uint32_t> &getBeachBlock() const { return beachBlock; }

    /** @throws DataDecodeException */
    static BiomeCappedSurfaceData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<std::uint32_t> floorBlocks;
    std::vector<std::uint32_t> ceilingBlocks;
    std::optional<std::uint32_t> seaBlock;
    std::optional<std::uint32_t> foundationBlock;
    std::optional<std::uint32_t> beachBlock;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
