/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ScorePacketEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace bedrock_protocol::types {

class ScorePacketEntry {
public:
    static constexpr std::uint8_t TYPE_PLAYER = 1;
    static constexpr std::uint8_t TYPE_ENTITY = 2;
    static constexpr std::uint8_t TYPE_FAKE_PLAYER = 3;

    std::int64_t scoreboardId = 0;
    std::string objectiveName;
    std::int32_t score = 0;
    std::uint8_t type = 0;
    /** @note (if type entity or player) */
    std::optional<std::int64_t> actorUniqueId;
    /** @note (if type fake player) */
    std::optional<std::string> customName;
};

}  // namespace bedrock_protocol::types
