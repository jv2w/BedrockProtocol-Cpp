/*
 * This file is part of BedrockProtocol-Cpp.
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
    //gophertunnel minecraft/protocol/recipe.go:76-83 - the context leads, and the ingredient list is only
    //present when the context is CONTEXT_NONE.
    const auto context = VarInt::readSignedInt(in);
    const auto present = CommonTypes::getBool(in);
    std::vector<RecipeIngredient> unlockingIngredients;
    if (present) {
        for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; i++) {
            unlockingIngredients.push_back(CommonTypes::getRecipeIngredient(in));
        }
    }

    return RecipeUnlockingRequirement(context, std::move(unlockingIngredients));
}

void RecipeUnlockingRequirement::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, context);
    const auto present = context == CONTEXT_NONE;
    CommonTypes::putBool(out, present);
    if (present) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(unlockingIngredients.size()));
        for (const auto &ingredient : unlockingIngredients) {
            CommonTypes::putRecipeIngredient(out, ingredient);
        }
    }
}

}  // namespace bedrock_protocol::types::recipe
