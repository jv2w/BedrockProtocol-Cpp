/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/InputMode.php
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

class InputMode final {
public:
    InputMode() = delete;

    static constexpr std::uint32_t MOUSE_KEYBOARD = 1;
    static constexpr std::uint32_t TOUCHSCREEN = 2;
    static constexpr std::uint32_t GAME_PAD = 3;
};

}  // namespace bedrock_protocol::types
