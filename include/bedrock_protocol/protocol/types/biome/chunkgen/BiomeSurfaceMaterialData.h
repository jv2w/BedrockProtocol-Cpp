/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeSurfaceMaterialData.php
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

class BiomeSurfaceMaterialData final {
public:
    BiomeSurfaceMaterialData(std::uint32_t topBlock, std::uint32_t midBlock, std::uint32_t seaFloorBlock,
                             std::uint32_t foundationBlock, std::uint32_t seaBlock, std::int32_t seaFloorDepth)
        : topBlock(topBlock),
          midBlock(midBlock),
          seaFloorBlock(seaFloorBlock),
          foundationBlock(foundationBlock),
          seaBlock(seaBlock),
          seaFloorDepth(seaFloorDepth)
    {
    }

    [[nodiscard]] std::uint32_t getTopBlock() const { return topBlock; }

    [[nodiscard]] std::uint32_t getMidBlock() const { return midBlock; }

    [[nodiscard]] std::uint32_t getSeaFloorBlock() const { return seaFloorBlock; }

    [[nodiscard]] std::uint32_t getFoundationBlock() const { return foundationBlock; }

    [[nodiscard]] std::uint32_t getSeaBlock() const { return seaBlock; }

    [[nodiscard]] std::int32_t getSeaFloorDepth() const { return seaFloorDepth; }

    /** @throws DataDecodeException */
    static BiomeSurfaceMaterialData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t topBlock;
    std::uint32_t midBlock;
    std::uint32_t seaFloorBlock;
    std::uint32_t foundationBlock;
    std::uint32_t seaBlock;
    std::int32_t seaFloorDepth;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
