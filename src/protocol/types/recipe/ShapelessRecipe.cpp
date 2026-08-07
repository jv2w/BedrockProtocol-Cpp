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

#include "bedrock_protocol/protocol/types/recipe/ShapelessRecipe.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::VarInt;
using serializer::CommonTypes;

ShapelessRecipe ShapelessRecipe::decode(encoding::ByteBufferReader &in)
{
    auto recipeId = CommonTypes::getString(in);
    std::vector<RecipeIngredient> input;
    for (std::uint32_t j = 0, ingredientCount = VarInt::readUnsignedInt(in); j < ingredientCount; ++j) {
        input.push_back(CommonTypes::getRecipeIngredient(in));
    }
    std::vector<inventory::ItemStack> output;
    for (std::uint32_t k = 0, resultCount = VarInt::readUnsignedInt(in); k < resultCount; ++k) {
        output.push_back(CommonTypes::getItemStackWithoutStackId(in));
    }
    const auto uuid = CommonTypes::getUUID(in);
    auto block = CommonTypes::getString(in);
    const auto priority = VarInt::readSignedInt(in);
    //gophertunnel minecraft/protocol/recipe.go:298 - the requirement is wrapped in an optional.
    auto unlockingRequirement = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &reader) { return RecipeUnlockingRequirement::read(reader); });

    const auto recipeNetId = CommonTypes::readRecipeNetId(in);

    return ShapelessRecipe(std::move(recipeId), std::move(input), std::move(output), uuid, std::move(block), priority,
                           std::move(unlockingRequirement), recipeNetId);
}

void ShapelessRecipe::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, recipeId);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(inputs.size()));
    for (const auto &item : inputs) {
        CommonTypes::putRecipeIngredient(out, item);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(outputs.size()));
    for (const auto &item : outputs) {
        CommonTypes::putItemStackWithoutStackId(out, item);
    }

    CommonTypes::putUUID(out, uuid);
    CommonTypes::putString(out, blockName);
    VarInt::writeSignedInt(out, priority);
    CommonTypes::writeOptional(
        out, unlockingRequirement,
        [](encoding::ByteBufferWriter &writer, const RecipeUnlockingRequirement &value) { value.write(writer); });

    CommonTypes::writeRecipeNetId(out, recipeNetId);
}

}  // namespace bedrock_protocol::types::recipe
