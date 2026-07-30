/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ChunkPosition.php
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

class ChunkPosition final {
public:
    ChunkPosition(std::int32_t x, std::int32_t z) : x(x), z(z) {}

    [[nodiscard]] std::int32_t getX() const { return x; }

    [[nodiscard]] std::int32_t getZ() const { return z; }

    static ChunkPosition read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t x;
    std::int32_t z;
};

}  // namespace bedrock_protocol::types
