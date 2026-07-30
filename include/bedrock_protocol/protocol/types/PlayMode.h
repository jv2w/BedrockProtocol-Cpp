/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayMode.php
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

/**
 * Enum used by PlayerAuthInputPacket. Most of these names don't make any sense, but that isn't surprising.
 */
class PlayMode final {
    PlayMode()
    {
        //NOOP
    }

public:
    static constexpr std::int32_t NORMAL = 0;
    static constexpr std::int32_t TEASER = 1;
    static constexpr std::int32_t SCREEN = 2;
    static constexpr std::int32_t EXIT_LEVEL = 7;
};

}  // namespace bedrock_protocol::types
