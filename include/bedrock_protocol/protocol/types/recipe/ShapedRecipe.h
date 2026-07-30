/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/ShapedRecipe.php
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
#include <string>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeUnlockingRequirement.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeWithTypeId.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::recipe {

class ShapedRecipe final : public RecipeWithTypeId {
public:
    /** @throws std::invalid_argument */
    ShapedRecipe(std::int32_t typeId, std::string recipeId, std::vector<std::vector<RecipeIngredient>> input,
                 std::vector<inventory::ItemStack> output, uuid::Uuid uuid,
                 std::string blockType,  //TODO: rename this
                 std::int32_t priority, bool symmetric, RecipeUnlockingRequirement unlockingRequirement,
                 std::uint32_t recipeNetId);

    [[nodiscard]] const std::string &getRecipeId() const { return recipeId; }

    [[nodiscard]] std::int32_t getWidth() const { return static_cast<std::int32_t>(input.at(0).size()); }

    [[nodiscard]] std::int32_t getHeight() const { return static_cast<std::int32_t>(input.size()); }

    [[nodiscard]] const std::vector<std::vector<RecipeIngredient>> &getInput() const { return input; }

    [[nodiscard]] const std::vector<inventory::ItemStack> &getOutput() const { return output; }

    [[nodiscard]] const uuid::Uuid &getUuid() const { return uuid; }

    [[nodiscard]] const std::string &getBlockName() const { return blockName; }

    [[nodiscard]] std::int32_t getPriority() const { return priority; }

    [[nodiscard]] bool isSymmetric() const { return symmetric; }

    [[nodiscard]] const RecipeUnlockingRequirement &getUnlockingRequirement() const { return unlockingRequirement; }

    [[nodiscard]] std::uint32_t getRecipeNetId() const { return recipeNetId; }

    /** @throws DataDecodeException */
    static ShapedRecipe decode(std::int32_t recipeType, encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<RecipeWithTypeId> clone() const override
    {
        return std::make_unique<ShapedRecipe>(*this);
    }

private:
    std::string blockName;

    std::string recipeId;
    std::vector<std::vector<RecipeIngredient>> input;
    std::vector<inventory::ItemStack> output;
    uuid::Uuid uuid;
    std::int32_t priority;
    bool symmetric;
    RecipeUnlockingRequirement unlockingRequirement;
    std::uint32_t recipeNetId;
};

}  // namespace bedrock_protocol::types::recipe
