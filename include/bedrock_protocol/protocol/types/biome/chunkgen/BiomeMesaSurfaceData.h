/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeMesaSurfaceData.php
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

class BiomeMesaSurfaceData final {
public:
    BiomeMesaSurfaceData(std::uint32_t clayMaterial, std::uint32_t hardClayMaterial, bool brycePillars, bool forest)
        : clayMaterial(clayMaterial), hardClayMaterial(hardClayMaterial), brycePillars(brycePillars), forest(forest)
    {
    }

    [[nodiscard]] std::uint32_t getClayMaterial() const { return clayMaterial; }

    [[nodiscard]] std::uint32_t getHardClayMaterial() const { return hardClayMaterial; }

    [[nodiscard]] bool hasBrycePillars() const { return brycePillars; }

    [[nodiscard]] bool hasForest() const { return forest; }

    /** @throws DataDecodeException */
    static BiomeMesaSurfaceData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t clayMaterial;
    std::uint32_t hardClayMaterial;
    bool brycePillars;
    bool forest;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
