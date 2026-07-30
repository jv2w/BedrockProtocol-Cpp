/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/MapDecoration.php
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

namespace bedrock_protocol::types {

class MapDecoration {
public:
    MapDecoration(std::uint8_t icon, std::uint8_t rotation, std::uint8_t xOffset, std::uint8_t yOffset,
                  std::string label, color::Color color)
        : icon(icon), rotation(rotation), xOffset(xOffset), yOffset(yOffset), label(std::move(label)), color(color)
    {
    }

    virtual ~MapDecoration() = default;

    [[nodiscard]] std::uint8_t getIcon() const
    {
        return icon;
    }

    [[nodiscard]] std::uint8_t getRotation() const
    {
        return rotation;
    }

    [[nodiscard]] std::uint8_t getXOffset() const
    {
        return xOffset;
    }

    [[nodiscard]] std::uint8_t getYOffset() const
    {
        return yOffset;
    }

    [[nodiscard]] const std::string &getLabel() const
    {
        return label;
    }

    [[nodiscard]] const color::Color &getColor() const
    {
        return color;
    }

private:
    std::uint8_t icon;
    std::uint8_t rotation;
    std::uint8_t xOffset;
    std::uint8_t yOffset;
    std::string label;
    color::Color color;
};

}  // namespace bedrock_protocol::types
