/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/FurnaceRecipeBlockName.php
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

class FurnaceRecipeBlockName final {
public:
    FurnaceRecipeBlockName() = delete;

    static constexpr std::string_view BLAST_FURNACE = "blast_furnace";
    static constexpr std::string_view CAMPFIRE = "campfire";
    static constexpr std::string_view FURNACE = "furnace";
    static constexpr std::string_view SMOKER = "smoker";
    static constexpr std::string_view SOUL_CAMPFIRE = "soul_campfire";
};

}  // namespace bedrock_protocol::types::recipe
