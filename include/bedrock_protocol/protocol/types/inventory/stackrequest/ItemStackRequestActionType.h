/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/ItemStackRequestActionType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::inventory::stackrequest {

class ItemStackRequestActionType final {
public:
    ItemStackRequestActionType() = delete;

    static constexpr std::int32_t TAKE = 0;
    static constexpr std::int32_t PLACE = 1;
    static constexpr std::int32_t SWAP = 2;
    static constexpr std::int32_t DROP = 3;
    static constexpr std::int32_t DESTROY = 4;
    static constexpr std::int32_t CRAFTING_CONSUME_INPUT = 5;
    static constexpr std::int32_t CRAFTING_CREATE_SPECIFIC_RESULT = 6;
    static constexpr std::int32_t LAB_TABLE_COMBINE = 9;
    static constexpr std::int32_t BEACON_PAYMENT = 10;
    static constexpr std::int32_t MINE_BLOCK = 11;
    static constexpr std::int32_t CRAFTING_RECIPE = 12;
    static constexpr std::int32_t CRAFTING_RECIPE_AUTO = 13; //recipe book?
    static constexpr std::int32_t CREATIVE_CREATE = 14;
    static constexpr std::int32_t CRAFTING_RECIPE_OPTIONAL = 15; //anvil/cartography table rename
    static constexpr std::int32_t CRAFTING_GRINDSTONE = 16;
    static constexpr std::int32_t CRAFTING_LOOM = 17;
    static constexpr std::int32_t CRAFTING_NON_IMPLEMENTED_DEPRECATED_ASK_TY_LAING = 18;
    static constexpr std::int32_t CRAFTING_RESULTS_DEPRECATED_ASK_TY_LAING = 19; //no idea what this is for
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
