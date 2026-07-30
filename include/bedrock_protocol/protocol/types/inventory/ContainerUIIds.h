/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/ContainerUIIds.php
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

class ContainerUIIds final {
public:
    ContainerUIIds() = delete;
    //NOOP

    static constexpr std::int32_t ANVIL_INPUT = 0;
    static constexpr std::int32_t ANVIL_MATERIAL = 1;
    static constexpr std::int32_t ANVIL_RESULT_PREVIEW = 2;
    static constexpr std::int32_t SMITHING_TABLE_INPUT = 3;
    static constexpr std::int32_t SMITHING_TABLE_MATERIAL = 4;
    static constexpr std::int32_t SMITHING_TABLE_RESULT_PREVIEW = 5;
    static constexpr std::int32_t ARMOR = 6;
    static constexpr std::int32_t LEVEL_ENTITY = 7;
    static constexpr std::int32_t BEACON_PAYMENT = 8;
    static constexpr std::int32_t BREWING_STAND_INPUT = 9;
    static constexpr std::int32_t BREWING_STAND_RESULT = 10;
    static constexpr std::int32_t BREWING_STAND_FUEL = 11;
    static constexpr std::int32_t COMBINED_HOTBAR_AND_INVENTORY = 12;
    static constexpr std::int32_t CRAFTING_INPUT = 13;
    static constexpr std::int32_t CRAFTING_OUTPUT_PREVIEW = 14;
    static constexpr std::int32_t RECIPE_CONSTRUCTION = 15;
    static constexpr std::int32_t RECIPE_NATURE = 16;
    static constexpr std::int32_t RECIPE_ITEMS = 17;
    static constexpr std::int32_t RECIPE_SEARCH = 18;
    static constexpr std::int32_t RECIPE_SEARCH_BAR = 19;
    static constexpr std::int32_t RECIPE_EQUIPMENT = 20;
    static constexpr std::int32_t RECIPE_BOOK = 21;
    static constexpr std::int32_t ENCHANTING_INPUT = 22;
    static constexpr std::int32_t ENCHANTING_MATERIAL = 23;
    static constexpr std::int32_t FURNACE_FUEL = 24;
    static constexpr std::int32_t FURNACE_INGREDIENT = 25;
    static constexpr std::int32_t FURNACE_RESULT = 26;
    static constexpr std::int32_t HORSE_EQUIP = 27;
    static constexpr std::int32_t HOTBAR = 28;
    static constexpr std::int32_t INVENTORY = 29;
    static constexpr std::int32_t SHULKER_BOX = 30;
    static constexpr std::int32_t TRADE_INGREDIENT1 = 31;
    static constexpr std::int32_t TRADE_INGREDIENT2 = 32;
    static constexpr std::int32_t TRADE_RESULT_PREVIEW = 33;
    static constexpr std::int32_t OFFHAND = 34;
    static constexpr std::int32_t COMPOUND_CREATOR_INPUT = 35;
    static constexpr std::int32_t COMPOUND_CREATOR_OUTPUT_PREVIEW = 36;
    static constexpr std::int32_t ELEMENT_CONSTRUCTOR_OUTPUT_PREVIEW = 37;
    static constexpr std::int32_t MATERIAL_REDUCER_INPUT = 38;
    static constexpr std::int32_t MATERIAL_REDUCER_OUTPUT = 39;
    static constexpr std::int32_t LAB_TABLE_INPUT = 40;
    static constexpr std::int32_t LOOM_INPUT = 41;
    static constexpr std::int32_t LOOM_DYE = 42;
    static constexpr std::int32_t LOOM_MATERIAL = 43;
    static constexpr std::int32_t LOOM_RESULT_PREVIEW = 44;
    static constexpr std::int32_t BLAST_FURNACE_INGREDIENT = 45;
    static constexpr std::int32_t SMOKER_INGREDIENT = 46;
    static constexpr std::int32_t TRADE2_INGREDIENT1 = 47;
    static constexpr std::int32_t TRADE2_INGREDIENT2 = 48;
    static constexpr std::int32_t TRADE2_RESULT_PREVIEW = 49;
    static constexpr std::int32_t GRINDSTONE_INPUT = 50;
    static constexpr std::int32_t GRINDSTONE_ADDITIONAL = 51;
    static constexpr std::int32_t GRINDSTONE_RESULT_PREVIEW = 52;
    static constexpr std::int32_t STONECUTTER_INPUT = 53;
    static constexpr std::int32_t STONECUTTER_RESULT_PREVIEW = 54;
    static constexpr std::int32_t CARTOGRAPHY_INPUT = 55;
    static constexpr std::int32_t CARTOGRAPHY_ADDITIONAL = 56;
    static constexpr std::int32_t CARTOGRAPHY_RESULT_PREVIEW = 57;
    static constexpr std::int32_t BARREL = 58;
    static constexpr std::int32_t CURSOR = 59;
    static constexpr std::int32_t CREATED_OUTPUT = 60;
    static constexpr std::int32_t SMITHING_TABLE_TEMPLATE = 61;
    static constexpr std::int32_t CRAFTER = 62;
    static constexpr std::int32_t DYNAMIC = 63;
    static constexpr std::int32_t RECIPE_FOOD_CONTAINER = 64;
    static constexpr std::int32_t RECIPE_BLOCKS_CONTAINER = 65;
    static constexpr std::int32_t RECIPE_FURNACE_ITEMS_CONTAINER = 66;
};

}  // namespace bedrock_protocol::types::inventory
