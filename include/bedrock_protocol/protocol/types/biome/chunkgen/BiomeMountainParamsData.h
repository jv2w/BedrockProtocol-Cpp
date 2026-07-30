/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeMountainParamsData.php
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

class BiomeMountainParamsData final {
public:
    BiomeMountainParamsData(std::uint32_t steepBlock, bool northSlopes, bool southSlopes, bool westSlopes,
                            bool eastSlopes, bool topSlideEnabled)
        : steepBlock(steepBlock),
          northSlopes(northSlopes),
          southSlopes(southSlopes),
          westSlopes(westSlopes),
          eastSlopes(eastSlopes),
          topSlideEnabled(topSlideEnabled)
    {
    }

    [[nodiscard]] std::uint32_t getSteepBlock() const { return steepBlock; }

    [[nodiscard]] bool hasNorthSlopes() const { return northSlopes; }

    [[nodiscard]] bool hasSouthSlopes() const { return southSlopes; }

    [[nodiscard]] bool hasWestSlopes() const { return westSlopes; }

    [[nodiscard]] bool hasEastSlopes() const { return eastSlopes; }

    [[nodiscard]] bool hasTopSlideEnabled() const { return topSlideEnabled; }

    /** @throws DataDecodeException */
    static BiomeMountainParamsData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t steepBlock;
    bool northSlopes;
    bool southSlopes;
    bool westSlopes;
    bool eastSlopes;
    bool topSlideEnabled;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
