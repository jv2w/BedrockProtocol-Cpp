/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandPermissions.php
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
#include <string_view>

namespace bedrock_protocol::types::command {

class CommandPermissions final {
public:
    CommandPermissions() = delete; //NOOP

    static constexpr std::int32_t NORMAL = 0;
    static constexpr std::int32_t OPERATOR = 1;
    static constexpr std::int32_t AUTOMATION = 2; //command blocks
    static constexpr std::int32_t HOST = 3; //hosting player on LAN multiplayer
    static constexpr std::int32_t OWNER = 4; //server terminal on BDS
    static constexpr std::int32_t INTERNAL = 5;

    static std::string toName(std::int32_t value);

    static std::int32_t fromName(std::string_view name);
};

}  // namespace bedrock_protocol::types::command
