/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/hud/HudVisibility.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::hud {

enum class HudVisibility : std::int32_t {
    HIDE = 0,
    RESET = 1,
};

/**
 * @throws PacketDecodeException
 */
HudVisibility HudVisibilityFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::hud
