/*
 * This file is part of BedrockProtocol for Endstone.
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

#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol::types {

class MapTrackedObject {
public:
    static constexpr std::uint32_t TYPE_ENTITY = 0;
    static constexpr std::uint32_t TYPE_BLOCK = 1;

    std::uint32_t type = 0;

    /** @note Only set if is TYPE_ENTITY */
    std::int64_t actorUniqueId = 0;

    /** Only set if is TYPE_BLOCK */
    BlockPosition blockPosition;
};

}  // namespace bedrock_protocol::types
