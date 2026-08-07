/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/GameRuleType.php
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

struct GameRuleType final {
    GameRuleType() = delete;
    //NOOP

    /** New in 1.26.40; carries no payload. gophertunnel v1.58.0 minecraft/protocol/reader.go:272-273. */
    static constexpr std::int32_t NULL_ = 0;
    static constexpr std::int32_t BOOL = 1;
    static constexpr std::int32_t INT = 2;
    static constexpr std::int32_t FLOAT = 3;
};

}  // namespace bedrock_protocol::types
