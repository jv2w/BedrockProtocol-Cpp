/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/MapImage.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class MapImage final {
public:
    //these limits are enforced in the protocol in 1.20.0
    static constexpr std::int32_t MAX_HEIGHT = 128;
    static constexpr std::int32_t MAX_WIDTH = 128;

    /**
     * @param pixels
     * @throws std::invalid_argument
     */
    explicit MapImage(std::vector<std::vector<color::Color>> pixels);

    [[nodiscard]] std::int32_t getWidth() const { return width; }

    [[nodiscard]] std::int32_t getHeight() const { return height; }

    [[nodiscard]] const std::vector<std::vector<color::Color>> &getPixels() const { return pixels; }

    void encode(encoding::ByteBufferWriter &out) const;

    /**
     * @throws PacketDecodeException
     * @throws DataDecodeException
     */
    static MapImage decode(encoding::ByteBufferReader &in, std::int32_t height, std::int32_t width);

private:
    std::int32_t width;
    std::int32_t height;
    std::vector<std::vector<color::Color>> pixels;
    mutable std::optional<std::string> encodedPixelCache = std::nullopt;
};

}  // namespace bedrock_protocol::types
