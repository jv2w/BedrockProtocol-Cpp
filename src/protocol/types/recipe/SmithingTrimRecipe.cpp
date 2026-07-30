/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/SmithingTrimRecipe.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/SmithingTrimRecipe.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using serializer::CommonTypes;

SmithingTrimRecipe SmithingTrimRecipe::decode(std::int32_t typeId, encoding::ByteBufferReader &in)
{
    auto recipeId = CommonTypes::getString(in);
    auto template_ = CommonTypes::getRecipeIngredient(in);
    auto input = CommonTypes::getRecipeIngredient(in);
    auto addition = CommonTypes::getRecipeIngredient(in);
    auto blockName = CommonTypes::getString(in);
    const auto recipeNetId = CommonTypes::readRecipeNetId(in);

    return SmithingTrimRecipe(typeId, std::move(recipeId), std::move(template_), std::move(input),
                              std::move(addition), std::move(blockName), recipeNetId);
}

void SmithingTrimRecipe::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, recipeId);
    CommonTypes::putRecipeIngredient(out, template_);
    CommonTypes::putRecipeIngredient(out, input);
    CommonTypes::putRecipeIngredient(out, addition);
    CommonTypes::putString(out, blockName);
    CommonTypes::writeRecipeNetId(out, recipeNetId);
}

}  // namespace bedrock_protocol::types::recipe
