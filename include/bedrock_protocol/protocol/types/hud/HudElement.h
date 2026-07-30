/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/hud/HudElement.php
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

enum class HudElement : std::int32_t {
    PAPER_DOLL = 0,
    ARMOR = 1,
    TOOLTIPS = 2,
    TOUCH_CONTROLS = 3,
    CROSSHAIR = 4,
    HOTBAR = 5,
    HEALTH = 6,
    XP = 7,
    FOOD = 8,
    AIR_BUBBLES = 9,
    HORSE_HEALTH = 10,
    STATUS_EFFECTS = 11,
    ITEM_TEXT = 12,
};

/**
 * @throws PacketDecodeException
 */
HudElement HudElementFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::hud
