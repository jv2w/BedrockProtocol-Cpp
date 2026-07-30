/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/WindowTypes.php
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

class WindowTypes final {
public:
    WindowTypes() = delete;
    //NOOP

    static constexpr std::int32_t NONE = -9;

    static constexpr std::int32_t INVENTORY = -1;
    static constexpr std::int32_t CONTAINER = 0;
    static constexpr std::int32_t WORKBENCH = 1;
    static constexpr std::int32_t FURNACE = 2;
    static constexpr std::int32_t ENCHANTMENT = 3;
    static constexpr std::int32_t BREWING_STAND = 4;
    static constexpr std::int32_t ANVIL = 5;
    static constexpr std::int32_t DISPENSER = 6;
    static constexpr std::int32_t DROPPER = 7;
    static constexpr std::int32_t HOPPER = 8;
    static constexpr std::int32_t CAULDRON = 9;
    static constexpr std::int32_t MINECART_CHEST = 10;
    static constexpr std::int32_t MINECART_HOPPER = 11;
    static constexpr std::int32_t HORSE = 12;
    static constexpr std::int32_t BEACON = 13;
    static constexpr std::int32_t STRUCTURE_EDITOR = 14;
    static constexpr std::int32_t TRADING = 15;
    static constexpr std::int32_t COMMAND_BLOCK = 16;
    static constexpr std::int32_t JUKEBOX = 17;
    static constexpr std::int32_t ARMOR = 18;
    static constexpr std::int32_t HAND = 19;
    static constexpr std::int32_t COMPOUND_CREATOR = 20;
    static constexpr std::int32_t ELEMENT_CONSTRUCTOR = 21;
    static constexpr std::int32_t MATERIAL_REDUCER = 22;
    static constexpr std::int32_t LAB_TABLE = 23;
    static constexpr std::int32_t LOOM = 24;
    static constexpr std::int32_t LECTERN = 25;
    static constexpr std::int32_t GRINDSTONE = 26;
    static constexpr std::int32_t BLAST_FURNACE = 27;
    static constexpr std::int32_t SMOKER = 28;
    static constexpr std::int32_t STONECUTTER = 29;
    static constexpr std::int32_t CARTOGRAPHY = 30;
    static constexpr std::int32_t HUD = 31;
    static constexpr std::int32_t JIGSAW_EDITOR = 32;
    static constexpr std::int32_t SMITHING_TABLE = 33;
    static constexpr std::int32_t CHEST_BOAT = 34;
    static constexpr std::int32_t DECORATED_POT = 35;
    static constexpr std::int32_t CRAFTER = 36;
};

}  // namespace bedrock_protocol::types::inventory
