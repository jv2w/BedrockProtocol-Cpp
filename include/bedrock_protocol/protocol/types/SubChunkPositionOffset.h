/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPositionOffset.php
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

class SubChunkPositionOffset final {
public:
    SubChunkPositionOffset(std::int32_t xOffset, std::int32_t yOffset, std::int32_t zOffset);

    [[nodiscard]] std::int8_t getXOffset() const { return xOffset; }

    [[nodiscard]] std::int8_t getYOffset() const { return yOffset; }

    [[nodiscard]] std::int8_t getZOffset() const { return zOffset; }

    static SubChunkPositionOffset read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    static void clampOffset(std::int32_t v);

    std::int8_t xOffset;
    std::int8_t yOffset;
    std::int8_t zOffset;
};

}  // namespace bedrock_protocol::types
