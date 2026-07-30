/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MemoryCategoryCounter.php
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

class MemoryCategoryCounter final {
public:
    MemoryCategoryCounter(std::uint8_t category, std::uint64_t bytes) : category(category), bytes(bytes) {}

    /**
     * @see MemoryCategory
     */
    [[nodiscard]] std::uint8_t getCategory() const { return category; }

    [[nodiscard]] std::uint64_t getBytes() const { return bytes; }

    /**
     * @throws DataDecodeException
     */
    static MemoryCategoryCounter read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t category;
    std::uint64_t bytes;
};

}  // namespace bedrock_protocol::types
