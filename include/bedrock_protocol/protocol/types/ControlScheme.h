/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ControlScheme.php
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
 * @see ClientboundControlSchemeSetPacket
 */
enum class ControlScheme : std::int32_t {
    LOCKED_PLAYER_RELATIVE_STRAFE = 0,
    CAMERA_RELATIVE = 1,
    CAMERA_RELATIVE_STRAFE = 2,
    PLAYER_RELATIVE = 3,
    PLAYER_RELATIVE_STRAFE = 4,
};

/**
 * @throws PacketDecodeException
 */
ControlScheme ControlSchemeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
