/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ContainerIds.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::inventory {

class ContainerIds final {
public:
    ContainerIds() = delete;
    //NOOP

    static constexpr std::int32_t NONE = -1;
    static constexpr std::int32_t INVENTORY = 0;
    static constexpr std::int32_t FIRST = 1;
    static constexpr std::int32_t LAST = 100;
    static constexpr std::int32_t OFFHAND = 119;
    static constexpr std::int32_t ARMOR = 120;

    static constexpr std::int32_t HOTBAR = 122;
    static constexpr std::int32_t FIXED_INVENTORY = 123;
    static constexpr std::int32_t UI = 124;
    static constexpr std::int32_t CONTAINER_ID_REGISTRY = 125;
};

}  // namespace bedrock_protocol::types::inventory
