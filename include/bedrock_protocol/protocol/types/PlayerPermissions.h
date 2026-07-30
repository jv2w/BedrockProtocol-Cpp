/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayerPermissions.php
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

class PlayerPermissions final {
    PlayerPermissions()
    {
        //NOOP
    }

public:
    static constexpr std::int32_t CUSTOM = 3;
    static constexpr std::int32_t OPERATOR = 2;
    static constexpr std::int32_t MEMBER = 1;
    static constexpr std::int32_t VISITOR = 0;
};

}  // namespace bedrock_protocol::types
