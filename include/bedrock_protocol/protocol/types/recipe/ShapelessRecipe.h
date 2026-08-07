/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/ShapelessRecipe.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeUnlockingRequirement.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::recipe {

class ShapelessRecipe final {
public:
    ShapelessRecipe(std::string recipeId, std::vector<RecipeIngredient> inputs,
                    std::vector<inventory::ItemStack> outputs, uuid::Uuid uuid, std::string blockName,
                    std::int32_t priority, std::optional<RecipeUnlockingRequirement> unlockingRequirement,
                    std::uint32_t recipeNetId)
        : recipeId(std::move(recipeId)), inputs(std::move(inputs)), outputs(std::move(outputs)), uuid(uuid),
          blockName(std::move(blockName)), priority(priority),
          unlockingRequirement(std::move(unlockingRequirement)), recipeNetId(recipeNetId)
    {
    }

    [[nodiscard]] const std::string &getRecipeId() const { return recipeId; }

    [[nodiscard]] const std::vector<RecipeIngredient> &getInputs() const { return inputs; }

    [[nodiscard]] const std::vector<inventory::ItemStack> &getOutputs() const { return outputs; }

    [[nodiscard]] const uuid::Uuid &getUuid() const { return uuid; }

    [[nodiscard]] const std::string &getBlockName() const { return blockName; }

    [[nodiscard]] std::int32_t getPriority() const { return priority; }

    [[nodiscard]] const std::optional<RecipeUnlockingRequirement> &getUnlockingRequirement() const
    {
        return unlockingRequirement;
    }

    [[nodiscard]] std::uint32_t getRecipeNetId() const { return recipeNetId; }

    /** @throws DataDecodeException */
    static ShapelessRecipe decode(encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const;

private:
    std::string recipeId;
    std::vector<RecipeIngredient> inputs;
    std::vector<inventory::ItemStack> outputs;
    uuid::Uuid uuid;
    std::string blockName;
    std::int32_t priority;
    std::optional<RecipeUnlockingRequirement> unlockingRequirement;
    std::uint32_t recipeNetId;
};

}  // namespace bedrock_protocol::types::recipe
