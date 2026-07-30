/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClocksType.php
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

/**
 * @see SyncWorldClocksPayload
 */
struct SyncWorldClocksType final {
    SyncWorldClocksType() = delete;

    static constexpr std::int32_t SYNC_STATE = 0;
    static constexpr std::int32_t INITIALIZE_REGISTRY = 1;
    static constexpr std::int32_t ADD_TIME_MARKER = 2;
    static constexpr std::int32_t REMOVE_TIME_MARKER = 3;
};

}  // namespace bedrock_protocol::types
