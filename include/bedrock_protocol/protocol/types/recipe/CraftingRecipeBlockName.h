/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/CraftingRecipeBlockName.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string_view>

namespace bedrock_protocol::types::recipe {

class CraftingRecipeBlockName final {
public:
    CraftingRecipeBlockName() = delete;

    static constexpr std::string_view CARTOGRAPHY_TABLE = "cartography_table";
    static constexpr std::string_view CRAFTING_TABLE = "crafting_table";
    static constexpr std::string_view STONECUTTER = "stonecutter";
    static constexpr std::string_view SMITHING_TABLE = "smithing_table";
};

}  // namespace bedrock_protocol::types::recipe
