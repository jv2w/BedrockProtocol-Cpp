/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ScoreboardIdentityPacketEntry.php
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

namespace bedrock_protocol::types {

class ScoreboardIdentityPacketEntry {
public:
    std::int64_t scoreboardId = 0;
    std::optional<std::int64_t> actorUniqueId = std::nullopt;
};

}  // namespace bedrock_protocol::types
