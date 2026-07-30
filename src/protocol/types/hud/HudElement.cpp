/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/hud/HudElement.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/hud/HudElement.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::hud {

HudElement HudElementFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return HudElement::PAPER_DOLL;
    case 1:
        return HudElement::ARMOR;
    case 2:
        return HudElement::TOOLTIPS;
    case 3:
        return HudElement::TOUCH_CONTROLS;
    case 4:
        return HudElement::CROSSHAIR;
    case 5:
        return HudElement::HOTBAR;
    case 6:
        return HudElement::HEALTH;
    case 7:
        return HudElement::XP;
    case 8:
        return HudElement::FOOD;
    case 9:
        return HudElement::AIR_BUBBLES;
    case 10:
        return HudElement::HORSE_HEALTH;
    case 11:
        return HudElement::STATUS_EFFECTS;
    case 12:
        return HudElement::ITEM_TEXT;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for HudElement");
    }
}

}  // namespace bedrock_protocol::types::hud
