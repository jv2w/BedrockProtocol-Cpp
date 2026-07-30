/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/DebugMarkerData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol::types {

class DebugMarkerData final {
public:
    DebugMarkerData(std::string text, const math::Vector3 &position, const color::Color &color,
                    std::uint64_t durationMillis)
        : text(std::move(text)), position(position), color(color), durationMillis(durationMillis)
    {
    }

    [[nodiscard]] const std::string &getText() const { return text; }

    [[nodiscard]] const math::Vector3 &getPosition() const { return position; }

    [[nodiscard]] const color::Color &getColor() const { return color; }

    [[nodiscard]] std::uint64_t getDurationMillis() const { return durationMillis; }

    static DebugMarkerData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string text;
    math::Vector3 position;
    color::Color color;
    std::uint64_t durationMillis;
};

}  // namespace bedrock_protocol::types
