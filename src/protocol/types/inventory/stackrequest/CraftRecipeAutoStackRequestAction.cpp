/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CraftRecipeAutoStackRequestAction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftRecipeAutoStackRequestAction.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory::stackrequest {

using encoding::Byte;
using serializer::CommonTypes;

CraftRecipeAutoStackRequestAction CraftRecipeAutoStackRequestAction::read(encoding::ByteBufferReader &in) {
    const auto recipeId = CommonTypes::readRecipeNetId(in);
    const auto repetitions = Byte::readUnsigned(in);
    const auto repetitions2 = Byte::readUnsigned(in); //repetitions property is sent twice, mojang...
    std::vector<recipe::RecipeIngredient> ingredients;
    for (std::uint8_t i = 0, count = Byte::readUnsigned(in); i < count; ++i) {
        ingredients.push_back(CommonTypes::getRecipeIngredient(in));
    }
    return CraftRecipeAutoStackRequestAction(recipeId, repetitions, repetitions2, std::move(ingredients));
}

void CraftRecipeAutoStackRequestAction::write(encoding::ByteBufferWriter &out) const {
    CommonTypes::writeRecipeNetId(out, recipeId);
    Byte::writeUnsigned(out, repetitions);
    Byte::writeUnsigned(out, repetitions2);
    Byte::writeUnsigned(out, static_cast<std::uint8_t>(ingredients.size()));
    for (const auto &ingredient : ingredients) {
        CommonTypes::putRecipeIngredient(out, ingredient);
    }
}

std::unique_ptr<ItemStackRequestAction> CraftRecipeAutoStackRequestAction::clone() const {
    return std::make_unique<CraftRecipeAutoStackRequestAction>(*this);
}

}  // namespace bedrock_protocol::types::inventory::stackrequest
