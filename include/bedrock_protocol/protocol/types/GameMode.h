/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/GameMode.php
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

struct GameMode final {
    GameMode() = delete;
    //NOOP

    static constexpr std::int32_t SURVIVAL = 0;
    static constexpr std::int32_t CREATIVE = 1;
    static constexpr std::int32_t ADVENTURE = 2;
    static constexpr std::int32_t SURVIVAL_VIEWER = 3;
    static constexpr std::int32_t CREATIVE_VIEWER = 4;
    static constexpr std::int32_t DEFAULT = 5;
    static constexpr std::int32_t SPECTATOR = 6;
};

}  // namespace bedrock_protocol::types
