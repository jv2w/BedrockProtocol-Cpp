/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/SmithingTransformRecipe.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeWithTypeId.h"

namespace bedrock_protocol::types::recipe {

class SmithingTransformRecipe final : public RecipeWithTypeId {
public:
    SmithingTransformRecipe(std::int32_t typeId, std::string recipeId, RecipeIngredient template_,
                            RecipeIngredient input, RecipeIngredient addition, inventory::ItemStack output,
                            std::string blockName, std::uint32_t recipeNetId)
        : RecipeWithTypeId(typeId), recipeId(std::move(recipeId)), template_(std::move(template_)),
          input(std::move(input)), addition(std::move(addition)), output(std::move(output)),
          blockName(std::move(blockName)), recipeNetId(recipeNetId)
    {
    }

    [[nodiscard]] const std::string &getRecipeId() const { return recipeId; }

    /** Named getTemplate() as in PHP; the field is named template_ because "template" is a C++ keyword. */
    [[nodiscard]] const RecipeIngredient &getTemplate() const { return template_; }

    [[nodiscard]] const RecipeIngredient &getInput() const { return input; }

    [[nodiscard]] const RecipeIngredient &getAddition() const { return addition; }

    [[nodiscard]] const inventory::ItemStack &getOutput() const { return output; }

    [[nodiscard]] const std::string &getBlockName() const { return blockName; }

    [[nodiscard]] std::uint32_t getRecipeNetId() const { return recipeNetId; }

    /** @throws DataDecodeException */
    static SmithingTransformRecipe decode(std::int32_t typeId, encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<RecipeWithTypeId> clone() const override
    {
        return std::make_unique<SmithingTransformRecipe>(*this);
    }

private:
    std::string recipeId;
    RecipeIngredient template_;
    RecipeIngredient input;
    RecipeIngredient addition;
    inventory::ItemStack output;
    std::string blockName;
    std::uint32_t recipeNetId;
};

}  // namespace bedrock_protocol::types::recipe
