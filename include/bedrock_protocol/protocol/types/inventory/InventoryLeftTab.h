/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/InventoryLeftTab.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::inventory {

enum class InventoryLeftTab : std::int32_t {
    NONE = 0,
    CONSTRUCTION = 1,
    EQUIPMENT = 2,
    ITEMS = 3,
    NATURE = 4,
    SEARCH = 5,
    SURVIVAL = 6,
};

/**
 * @throws PacketDecodeException
 */
InventoryLeftTab InventoryLeftTabFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::inventory
