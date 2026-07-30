/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/InventoryLayout.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/InventoryLayout.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::inventory {

InventoryLayout InventoryLayoutFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return InventoryLayout::NONE;
    case 1:
        return InventoryLayout::INVENTORY_ONLY;
    case 2:
        return InventoryLayout::DEFAULT;
    case 3:
        return InventoryLayout::RECIPE_BOOK_ONLY;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for InventoryLayout");
    }
}

}  // namespace bedrock_protocol::types::inventory
