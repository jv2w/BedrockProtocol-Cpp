/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/RecipeUnlockingRequirement.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/RecipeUnlockingRequirement.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::VarInt;
using serializer::CommonTypes;

RecipeUnlockingRequirement RecipeUnlockingRequirement::read(encoding::ByteBufferReader &in)
{
    //I don't know what the point of this structure is. It could easily have been a list<RecipeIngredient> instead.
    //It's basically just an optional list, which could have been done by an empty list wherever it's not needed.
    const auto unlockingContext = CommonTypes::getBool(in);
    std::optional<std::vector<RecipeIngredient>> unlockingIngredients;
    if (!unlockingContext) {
        unlockingIngredients.emplace();
        for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; i++) {
            unlockingIngredients->push_back(CommonTypes::getRecipeIngredient(in));
        }
    }

    return RecipeUnlockingRequirement(std::move(unlockingIngredients));
}

void RecipeUnlockingRequirement::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBool(out, !unlockingIngredients.has_value());
    if (unlockingIngredients.has_value()) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(unlockingIngredients->size()));
        for (const auto &ingredient : *unlockingIngredients) {
            CommonTypes::putRecipeIngredient(out, ingredient);
        }
    }
}

}  // namespace bedrock_protocol::types::recipe
