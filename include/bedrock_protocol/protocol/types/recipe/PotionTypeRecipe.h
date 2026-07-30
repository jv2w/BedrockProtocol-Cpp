/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/PotionTypeRecipe.php
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

class PotionTypeRecipe {
public:
    PotionTypeRecipe(std::int32_t inputItemId, std::int32_t inputItemMeta, std::int32_t ingredientItemId,
                     std::int32_t ingredientItemMeta, std::int32_t outputItemId, std::int32_t outputItemMeta)
        : inputItemId(inputItemId), inputItemMeta(inputItemMeta), ingredientItemId(ingredientItemId),
          ingredientItemMeta(ingredientItemMeta), outputItemId(outputItemId), outputItemMeta(outputItemMeta)
    {
    }

    virtual ~PotionTypeRecipe() = default;

    [[nodiscard]] std::int32_t getInputItemId() const { return inputItemId; }

    [[nodiscard]] std::int32_t getInputItemMeta() const { return inputItemMeta; }

    [[nodiscard]] std::int32_t getIngredientItemId() const { return ingredientItemId; }

    [[nodiscard]] std::int32_t getIngredientItemMeta() const { return ingredientItemMeta; }

    [[nodiscard]] std::int32_t getOutputItemId() const { return outputItemId; }

    [[nodiscard]] std::int32_t getOutputItemMeta() const { return outputItemMeta; }

private:
    std::int32_t inputItemId;
    std::int32_t inputItemMeta;
    std::int32_t ingredientItemId;
    std::int32_t ingredientItemMeta;
    std::int32_t outputItemId;
    std::int32_t outputItemMeta;
};

}  // namespace bedrock_protocol::types::recipe
