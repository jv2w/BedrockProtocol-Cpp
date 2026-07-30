/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/MaterialReducerRecipeOutput.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::recipe {

class MaterialReducerRecipeOutput final {
public:
    MaterialReducerRecipeOutput(std::int32_t itemId, std::int32_t count) : itemId(itemId), count(count) {}

    [[nodiscard]] std::int32_t getItemId() const { return itemId; }

    [[nodiscard]] std::int32_t getCount() const { return count; }

private:
    std::int32_t itemId;
    std::int32_t count;
};

}  // namespace bedrock_protocol::types::recipe
