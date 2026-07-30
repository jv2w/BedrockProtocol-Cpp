/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/skin/SkinImage.php
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

namespace bedrock_protocol::types::skin {

class SkinImage {
public:
    /**
     * @throws std::invalid_argument
     */
    SkinImage(std::uint32_t height, std::uint32_t width, std::string data);

    /**
     * @throws std::invalid_argument
     */
    static SkinImage fromLegacy(const std::string &data);

    [[nodiscard]] std::uint32_t getHeight() const { return height; }

    [[nodiscard]] std::uint32_t getWidth() const { return width; }

    [[nodiscard]] const std::string &getData() const { return data; }

private:
    std::uint32_t height;
    std::uint32_t width;
    std::string data;
};

}  // namespace bedrock_protocol::types::skin
