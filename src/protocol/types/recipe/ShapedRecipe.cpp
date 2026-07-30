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

#include "bedrock_protocol/protocol/types/recipe/ShapedRecipe.h"

#include <optional>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::VarInt;
using serializer::CommonTypes;

ShapedRecipe::ShapedRecipe(std::int32_t typeId, std::string recipeId,
                           std::vector<std::vector<RecipeIngredient>> input, std::vector<inventory::ItemStack> output,
                           uuid::Uuid uuid,
                           std::string blockType,  //TODO: rename this
                           std::int32_t priority, bool symmetric, RecipeUnlockingRequirement unlockingRequirement,
                           std::uint32_t recipeNetId)
    : RecipeWithTypeId(typeId), blockName(std::move(blockType)), recipeId(std::move(recipeId)),
      input(std::move(input)), output(std::move(output)), uuid(uuid), priority(priority), symmetric(symmetric),
      unlockingRequirement(std::move(unlockingRequirement)), recipeNetId(recipeNetId)
{
    const auto rows = this->input.size();
    if (rows < 1 || rows > 3) {
        throw std::invalid_argument("Expected 1, 2 or 3 input rows");
    }
    std::optional<std::size_t> columns;
    for (std::size_t rowNumber = 0; rowNumber < this->input.size(); ++rowNumber) {
        const auto &row = this->input[rowNumber];
        if (!columns.has_value()) {
            columns = row.size();
        }
        else if (row.size() != *columns) {
            throw std::invalid_argument("Expected each row to be " + std::to_string(*columns) +
                                        " columns, but have " + std::to_string(row.size()) + " in row " +
                                        std::to_string(rowNumber));
        }
    }
}

ShapedRecipe ShapedRecipe::decode(std::int32_t recipeType, encoding::ByteBufferReader &in)
{
    auto recipeId = CommonTypes::getString(in);
    const auto width = VarInt::readSignedInt(in);
    const auto height = VarInt::readSignedInt(in);
    std::vector<std::vector<RecipeIngredient>> input;
    for (std::int32_t row = 0; row < height; ++row) {
        for (std::int32_t column = 0; column < width; ++column) {
            //PHP auto-vivifies $input[$row] on first assignment, so a row only exists once it has a column.
            if (static_cast<std::int32_t>(input.size()) <= row) {
                input.emplace_back();
            }
            input[static_cast<std::size_t>(row)].push_back(CommonTypes::getRecipeIngredient(in));
        }
    }

    std::vector<inventory::ItemStack> output;
    for (std::uint32_t k = 0, resultCount = VarInt::readUnsignedInt(in); k < resultCount; ++k) {
        output.push_back(CommonTypes::getItemStackWithoutStackId(in));
    }
    const auto uuid = CommonTypes::getUUID(in);
    auto block = CommonTypes::getString(in);
    const auto priority = VarInt::readSignedInt(in);
    const auto symmetric = CommonTypes::getBool(in);
    auto unlockingRequirement = RecipeUnlockingRequirement::read(in);

    const auto recipeNetId = CommonTypes::readRecipeNetId(in);

    return ShapedRecipe(recipeType, std::move(recipeId), std::move(input), std::move(output), uuid, std::move(block),
                        priority, symmetric, std::move(unlockingRequirement), recipeNetId);
}

void ShapedRecipe::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, recipeId);
    VarInt::writeSignedInt(out, getWidth());
    VarInt::writeSignedInt(out, getHeight());
    for (const auto &row : input) {
        for (const auto &ingredient : row) {
            CommonTypes::putRecipeIngredient(out, ingredient);
        }
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(output.size()));
    for (const auto &item : output) {
        CommonTypes::putItemStackWithoutStackId(out, item);
    }

    CommonTypes::putUUID(out, uuid);
    CommonTypes::putString(out, blockName);
    VarInt::writeSignedInt(out, priority);
    CommonTypes::putBool(out, symmetric);
    unlockingRequirement.write(out);

    CommonTypes::writeRecipeNetId(out, recipeNetId);
}

}  // namespace bedrock_protocol::types::recipe
