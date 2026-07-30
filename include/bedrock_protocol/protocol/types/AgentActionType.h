/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AgentActionType.php
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

class AgentActionType final {
public:
    AgentActionType() = delete;  //NOOP

    static constexpr std::int32_t ATTACK = 1;
    static constexpr std::int32_t COLLECT = 2;
    static constexpr std::int32_t DESTROY = 3;
    static constexpr std::int32_t DETECT_REDSTONE = 4;
    static constexpr std::int32_t DETECT_OBSTACLE = 5;
    static constexpr std::int32_t DROP = 6;
    static constexpr std::int32_t DROP_ALL = 7;
    static constexpr std::int32_t INSPECT = 8;
    static constexpr std::int32_t INSPECT_DATA = 9;
    static constexpr std::int32_t INSPECT_ITEM_COUNT = 10;
    static constexpr std::int32_t INSPECT_ITEM_DETAIL = 11;
    static constexpr std::int32_t INSPECT_ITEM_SPACE = 12;
    static constexpr std::int32_t INTERACT = 13;
    static constexpr std::int32_t MOVE = 14;
    static constexpr std::int32_t PLACE_BLOCK = 15;
    static constexpr std::int32_t TILL = 16;
    static constexpr std::int32_t TRANSFER_ITEM_TO = 17;
    static constexpr std::int32_t TURN = 18;
};

}  // namespace bedrock_protocol::types
