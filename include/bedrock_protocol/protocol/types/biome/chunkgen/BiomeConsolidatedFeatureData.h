/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeConsolidatedFeatureData.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeScatterParamData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeConsolidatedFeatureData final {
public:
    BiomeConsolidatedFeatureData(BiomeScatterParamData scatter, std::int16_t feature, std::int16_t identifier,
                                 std::int16_t pass, bool useInternal)
        : scatter(std::move(scatter)),
          feature(feature),
          identifier(identifier),
          pass(pass),
          useInternal(useInternal)
    {
    }

    [[nodiscard]] const BiomeScatterParamData &getScatter() const { return scatter; }

    [[nodiscard]] std::int16_t getFeature() const { return feature; }

    [[nodiscard]] std::int16_t getIdentifier() const { return identifier; }

    [[nodiscard]] std::int16_t getPass() const { return pass; }

    [[nodiscard]] bool canUseInternal() const { return useInternal; }

    /** @throws DataDecodeException */
    static BiomeConsolidatedFeatureData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    BiomeScatterParamData scatter;
    std::int16_t feature;
    std::int16_t identifier;
    std::int16_t pass;
    bool useInternal;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
