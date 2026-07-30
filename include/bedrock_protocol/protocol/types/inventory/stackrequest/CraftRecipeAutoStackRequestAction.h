/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CraftRecipeAutoStackRequestAction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Tells that the current transaction crafted the specified recipe, using the recipe book. This is effectively the same
 * as the regular crafting result action.
 */
class CraftRecipeAutoStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_RECIPE_AUTO;

    CraftRecipeAutoStackRequestAction(std::uint32_t recipeId, std::uint8_t repetitions, std::uint8_t repetitions2,
                                      std::vector<recipe::RecipeIngredient> ingredients) :
        recipeId(recipeId),
        repetitions(repetitions),
        repetitions2(repetitions2),
        ingredients(std::move(ingredients)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint32_t getRecipeId() const { return recipeId; }

    [[nodiscard]] std::uint8_t getRepetitions() const { return repetitions; }

    [[nodiscard]] std::uint8_t getRepetitions2() const { return repetitions2; }

    [[nodiscard]] const std::vector<recipe::RecipeIngredient> &getIngredients() const { return ingredients; }

    static CraftRecipeAutoStackRequestAction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override;

private:
    std::uint32_t recipeId;
    std::uint8_t repetitions;
    std::uint8_t repetitions2;
    std::vector<recipe::RecipeIngredient> ingredients;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
