/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/InventoryLeftTab.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/InventoryLeftTab.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::inventory {

InventoryLeftTab InventoryLeftTabFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return InventoryLeftTab::NONE;
    case 1:
        return InventoryLeftTab::CONSTRUCTION;
    case 2:
        return InventoryLeftTab::EQUIPMENT;
    case 3:
        return InventoryLeftTab::ITEMS;
    case 4:
        return InventoryLeftTab::NATURE;
    case 5:
        return InventoryLeftTab::SEARCH;
    case 6:
        return InventoryLeftTab::SURVIVAL;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for InventoryLeftTab");
    }
}

}  // namespace bedrock_protocol::types::inventory
