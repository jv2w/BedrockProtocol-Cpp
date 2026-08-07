/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MapTrackedObject.php
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

#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol::types {

class MapTrackedObject {
public:
    static constexpr std::int32_t TYPE_ENTITY = 0;
    static constexpr std::int32_t TYPE_BLOCK = 1;

    std::int32_t type = 0;

    /**
     * Both members are optionals with their own presence byte, and BOTH are always written -
     * independently of `type`. gophertunnel v1.58.0 minecraft/protocol/map.go:51-58.
     */
    std::optional<std::int64_t> actorUniqueId = std::nullopt;

    std::optional<BlockPosition> blockPosition = std::nullopt;
};

}  // namespace bedrock_protocol::types
