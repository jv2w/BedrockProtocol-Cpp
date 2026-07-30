/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/BossBarColor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

class BossBarColor {
public:
    BossBarColor() = delete;

    static constexpr std::uint8_t PINK = 0;
    static constexpr std::uint8_t BLUE = 1;
    static constexpr std::uint8_t RED = 2;
    static constexpr std::uint8_t GREEN = 3;
    static constexpr std::uint8_t YELLOW = 4;
    static constexpr std::uint8_t PURPLE = 5;
    static constexpr std::uint8_t REBECCA_PURPLE = 6;
    static constexpr std::uint8_t WHITE = 7;
};

}  // namespace bedrock_protocol::types
