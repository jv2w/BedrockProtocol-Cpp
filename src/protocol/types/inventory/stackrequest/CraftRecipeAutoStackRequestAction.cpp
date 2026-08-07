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
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory::stackrequest {

using encoding::Byte;
using encoding::VarInt;
using serializer::CommonTypes;

CraftRecipeAutoStackRequestAction CraftRecipeAutoStackRequestAction::read(encoding::ByteBufferReader &in) {
    //gophertunnel minecraft/protocol/item_stack.go:510-514 - the duplicate repetition byte is gone, the
    //ingredient count is a varuint32, and the ingredients use the tagless stack-request descriptor framing.
    const auto recipeId = CommonTypes::readRecipeNetId(in);
    const auto repetitions = Byte::readUnsigned(in);
    std::vector<recipe::RecipeIngredient> ingredients;
    for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; ++i) {
        ingredients.push_back(CommonTypes::getStackRequestRecipeIngredient(in));
    }
    return CraftRecipeAutoStackRequestAction(recipeId, repetitions, std::move(ingredients));
}

void CraftRecipeAutoStackRequestAction::write(encoding::ByteBufferWriter &out) const {
    CommonTypes::writeRecipeNetId(out, recipeId);
    Byte::writeUnsigned(out, repetitions);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(ingredients.size()));
    for (const auto &ingredient : ingredients) {
        CommonTypes::putStackRequestRecipeIngredient(out, ingredient);
    }
}

std::unique_ptr<ItemStackRequestAction> CraftRecipeAutoStackRequestAction::clone() const {
    return std::make_unique<CraftRecipeAutoStackRequestAction>(*this);
}

}  // namespace bedrock_protocol::types::inventory::stackrequest
