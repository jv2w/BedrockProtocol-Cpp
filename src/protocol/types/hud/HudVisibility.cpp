/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/hud/HudVisibility.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/hud/HudVisibility.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::hud {

HudVisibility HudVisibilityFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return HudVisibility::HIDE;
    case 1:
        return HudVisibility::RESET;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for HudVisibility");
    }
}

}  // namespace bedrock_protocol::types::hud
