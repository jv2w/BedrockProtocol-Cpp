/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/MultiRecipe.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/MultiRecipe.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using serializer::CommonTypes;

MultiRecipe MultiRecipe::decode(encoding::ByteBufferReader &in)
{
    const auto uuid = CommonTypes::getUUID(in);
    const auto recipeNetId = CommonTypes::readRecipeNetId(in);
    return MultiRecipe(uuid, recipeNetId);
}

void MultiRecipe::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putUUID(out, recipeId);
    CommonTypes::writeRecipeNetId(out, recipeNetId);
}

}  // namespace bedrock_protocol::types::recipe
