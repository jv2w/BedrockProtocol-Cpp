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

#include "bedrock_protocol/protocol/types/ControlScheme.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

ControlScheme ControlSchemeFromPacket(std::int32_t value)
{
    switch (value) {
    case static_cast<std::int32_t>(ControlScheme::LOCKED_PLAYER_RELATIVE_STRAFE):
        return ControlScheme::LOCKED_PLAYER_RELATIVE_STRAFE;
    case static_cast<std::int32_t>(ControlScheme::CAMERA_RELATIVE):
        return ControlScheme::CAMERA_RELATIVE;
    case static_cast<std::int32_t>(ControlScheme::CAMERA_RELATIVE_STRAFE):
        return ControlScheme::CAMERA_RELATIVE_STRAFE;
    case static_cast<std::int32_t>(ControlScheme::PLAYER_RELATIVE):
        return ControlScheme::PLAYER_RELATIVE;
    case static_cast<std::int32_t>(ControlScheme::PLAYER_RELATIVE_STRAFE):
        return ControlScheme::PLAYER_RELATIVE_STRAFE;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for ControlScheme");
    }
}

}  // namespace bedrock_protocol::types
