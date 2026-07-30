/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MapInfoRequestPacketClientPixel.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class MapInfoRequestPacketClientPixel final {
public:
    color::Color color;
    std::uint16_t x;
    std::uint16_t y;

    MapInfoRequestPacketClientPixel(color::Color color, std::uint16_t x, std::uint16_t y) : color(color), x(x), y(y) {}

    [[nodiscard]] const color::Color &getColor() const { return color; }

    [[nodiscard]] std::uint16_t getX() const { return x; }

    [[nodiscard]] std::uint16_t getY() const { return y; }

    /**
     * @throws DataDecodeException
     */
    static MapInfoRequestPacketClientPixel read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    static constexpr std::uint16_t Y_INDEX_MULTIPLIER = 128;
};

}  // namespace bedrock_protocol::types
