/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ArmorSlot.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ArmorSlot.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

ArmorSlot ArmorSlotFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return ArmorSlot::HEAD;
    case 1:
        return ArmorSlot::TORSO;
    case 2:
        return ArmorSlot::LEGS;
    case 3:
        return ArmorSlot::FEET;
    case 4:
        return ArmorSlot::BODY;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for ArmorSlot");
    }
}

}  // namespace bedrock_protocol::types
