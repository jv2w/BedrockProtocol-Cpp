/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ChatRestrictionLevel.php
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

class ChatRestrictionLevel final {
public:
    ChatRestrictionLevel() = delete;

    static constexpr std::uint8_t NONE = 0;
    static constexpr std::uint8_t DROPPED = 1;
    static constexpr std::uint8_t DISABLED = 2;
};

}  // namespace bedrock_protocol::types
