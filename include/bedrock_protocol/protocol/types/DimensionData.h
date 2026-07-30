/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/DimensionData.php
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

namespace bedrock_protocol::types {

class DimensionData final {
public:
    DimensionData(std::int32_t maxHeight, std::int32_t minHeight, std::int32_t generator, std::int32_t dimensionType)
        : maxHeight(maxHeight), minHeight(minHeight), generator(generator), dimensionType(dimensionType)
    {
    }

    [[nodiscard]] std::int32_t getMaxHeight() const { return maxHeight; }

    [[nodiscard]] std::int32_t getMinHeight() const { return minHeight; }

    [[nodiscard]] std::int32_t getGenerator() const { return generator; }

    [[nodiscard]] std::int32_t getDimensionType() const { return dimensionType; }

    /** @throws DataDecodeException */
    static DimensionData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t maxHeight;
    std::int32_t minHeight;
    std::int32_t generator;
    std::int32_t dimensionType;
};

}  // namespace bedrock_protocol::types
