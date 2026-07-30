/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/OverrideUpdateType.php
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
 * @see PlayerUpdateEntityOverridesPacket
 */
enum class OverrideUpdateType : std::int32_t {
    CLEAR_OVERRIDES = 0,
    REMOVE_OVERRIDE = 1,
    SET_INT_OVERRIDE = 2,
    SET_FLOAT_OVERRIDE = 3,
};

/**
 * @throws PacketDecodeException
 */
OverrideUpdateType OverrideUpdateTypeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
