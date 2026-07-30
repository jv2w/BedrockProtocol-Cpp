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

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::inventory {

enum class InventoryLayout : std::int32_t {
    NONE = 0,
    INVENTORY_ONLY = 1,
    DEFAULT = 2,
    RECIPE_BOOK_ONLY = 3,
};

/**
 * @throws PacketDecodeException
 */
InventoryLayout InventoryLayoutFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::inventory
