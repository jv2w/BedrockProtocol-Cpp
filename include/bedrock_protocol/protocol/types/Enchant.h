/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/Enchant.php
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

class Enchant final {
public:
    Enchant(std::uint32_t id, std::uint8_t level) : id(id), level(level) {}

    [[nodiscard]] std::uint32_t getId() const { return id; }

    [[nodiscard]] std::uint8_t getLevel() const { return level; }

    /** @throws DataDecodeException */
    static Enchant read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t id;
    std::uint8_t level;
};

}  // namespace bedrock_protocol::types
