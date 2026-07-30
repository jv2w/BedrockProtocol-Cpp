/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/PotionContainerChangeRecipe.php
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

class PotionContainerChangeRecipe {
public:
    PotionContainerChangeRecipe(std::int32_t inputItemId, std::int32_t ingredientItemId, std::int32_t outputItemId)
        : inputItemId(inputItemId), ingredientItemId(ingredientItemId), outputItemId(outputItemId)
    {
    }

    virtual ~PotionContainerChangeRecipe() = default;

    [[nodiscard]] std::int32_t getInputItemId() const { return inputItemId; }

    [[nodiscard]] std::int32_t getIngredientItemId() const { return ingredientItemId; }

    [[nodiscard]] std::int32_t getOutputItemId() const { return outputItemId; }

private:
    std::int32_t inputItemId;
    std::int32_t ingredientItemId;
    std::int32_t outputItemId;
};

}  // namespace bedrock_protocol::types::recipe
