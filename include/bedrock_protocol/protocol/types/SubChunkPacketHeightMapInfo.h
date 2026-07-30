/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketHeightMapInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class SubChunkPacketHeightMapInfo {
public:
    /**
     * @param heights ZZZZXXXX key bit order
     */
    explicit SubChunkPacketHeightMapInfo(std::vector<std::int8_t> heights);

    [[nodiscard]] const std::vector<std::int8_t> &getHeights() const { return heights; }

    [[nodiscard]] std::int8_t getHeight(std::int32_t x, std::int32_t z) const
    {
        return heights[static_cast<std::size_t>(((z & 0xf) << 4) | (x & 0xf))];
    }

    static SubChunkPacketHeightMapInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

    static SubChunkPacketHeightMapInfo allTooLow();

    static SubChunkPacketHeightMapInfo allTooHigh();

    [[nodiscard]] bool isAllTooLow() const;

    [[nodiscard]] bool isAllTooHigh() const;

private:
    std::vector<std::int8_t> heights;
};

}  // namespace bedrock_protocol::types
