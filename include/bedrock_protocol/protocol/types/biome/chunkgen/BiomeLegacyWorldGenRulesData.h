/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeLegacyWorldGenRulesData final {
public:
    explicit BiomeLegacyWorldGenRulesData(std::vector<BiomeConditionalTransformationData> legacyPreHills)
        : legacyPreHills(std::move(legacyPreHills))
    {
    }

    [[nodiscard]] const std::vector<BiomeConditionalTransformationData> &getLegacyPreHills() const
    {
        return legacyPreHills;
    }

    /** @throws DataDecodeException */
    static BiomeLegacyWorldGenRulesData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<BiomeConditionalTransformationData> legacyPreHills;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
