/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/UIInventorySlotOffset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <unordered_map>

namespace bedrock_protocol::types::inventory {

/**
 * Values extracted from PlayerUISlot enum in Bedrock
 *
 * Deviation from PHP: the array-valued constants are PHP maps from UI slot ID to offset within the
 * corresponding container. They are pure lookup tables (never iterated for encoding), so they are
 * ported as std::unordered_map rather than an ordered vector of pairs.
 */
class UIInventorySlotOffset final {
public:
    using OffsetMap = std::unordered_map<std::int32_t, std::int32_t>;

    UIInventorySlotOffset() = delete;
    //NOOP

    static constexpr std::int32_t CURSOR = 0;
    static inline const OffsetMap ANVIL = {
        {1, 0},
        {2, 1},
    };
    static constexpr std::int32_t STONE_CUTTER_INPUT = 3;
    static inline const OffsetMap TRADE2_INGREDIENT = {
        {4, 0},
        {5, 1},
    };
    static inline const OffsetMap TRADE_INGREDIENT = {
        {6, 0},
        {7, 1},
    };
    static constexpr std::int32_t MATERIAL_REDUCER_INPUT = 8;
    static inline const OffsetMap LOOM = {
        {9, 0},
        {10, 1},
        {11, 2},
    };
    static inline const OffsetMap CARTOGRAPHY_TABLE = {
        {12, 0},
        {13, 1},
    };
    static inline const OffsetMap ENCHANTING_TABLE = {
        {14, 0},
        {15, 1},
    };
    static inline const OffsetMap GRINDSTONE = {
        {16, 0},
        {17, 1},
    };
    static inline const OffsetMap COMPOUND_CREATOR_INPUT = {
        {18, 0},
        {19, 1},
        {20, 2},
        {21, 3},
        {22, 4},
        {23, 5},
        {24, 6},
        {25, 7},
        {26, 8},
    };
    static constexpr std::int32_t BEACON_PAYMENT = 27;
    static inline const OffsetMap CRAFTING2X2_INPUT = {
        {28, 0},
        {29, 1},
        {30, 2},
        {31, 3},
    };
    static inline const OffsetMap CRAFTING3X3_INPUT = {
        {32, 0},
        {33, 1},
        {34, 2},
        {35, 3},
        {36, 4},
        {37, 5},
        {38, 6},
        {39, 7},
        {40, 8},
    };
    static inline const OffsetMap MATERIAL_REDUCER_OUTPUT = {
        {41, 0},
        {42, 1},
        {43, 2},
        {44, 3},
        {45, 4},
        {46, 5},
        {47, 6},
        {48, 7},
        {49, 8},
    };
    static constexpr std::int32_t CREATED_ITEM_OUTPUT = 50;
    static inline const OffsetMap SMITHING_TABLE = {
        {51, 0},  //input
        {52, 1},  //material
        {53, 2},  //template
    };
};

}  // namespace bedrock_protocol::types::inventory
