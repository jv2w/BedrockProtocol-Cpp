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

#include "bedrock_protocol/protocol/types/recipe/SmithingTransformRecipe.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using serializer::CommonTypes;

SmithingTransformRecipe SmithingTransformRecipe::decode(encoding::ByteBufferReader &in)
{
    auto recipeId = CommonTypes::getString(in);
    auto template_ = CommonTypes::getRecipeIngredient(in);
    auto input = CommonTypes::getRecipeIngredient(in);
    auto addition = CommonTypes::getRecipeIngredient(in);
    auto output = CommonTypes::getItemStackWithoutStackId(in);
    auto blockName = CommonTypes::getString(in);
    const auto recipeNetId = CommonTypes::readRecipeNetId(in);

    return SmithingTransformRecipe(std::move(recipeId), std::move(template_), std::move(input),
                                   std::move(addition), std::move(output), std::move(blockName), recipeNetId);
}

void SmithingTransformRecipe::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, recipeId);
    CommonTypes::putRecipeIngredient(out, template_);
    CommonTypes::putRecipeIngredient(out, input);
    CommonTypes::putRecipeIngredient(out, addition);
    CommonTypes::putItemStackWithoutStackId(out, output);
    CommonTypes::putString(out, blockName);
    CommonTypes::writeRecipeNetId(out, recipeNetId);
}

}  // namespace bedrock_protocol::types::recipe
