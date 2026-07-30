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

#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"

namespace bedrock_protocol::types::recipe {

class RecipeUnlockingRequirement final {
public:
    explicit RecipeUnlockingRequirement(std::optional<std::vector<RecipeIngredient>> unlockingIngredients)
        : unlockingIngredients(std::move(unlockingIngredients))
    {
    }

    [[nodiscard]] const std::optional<std::vector<RecipeIngredient>> &getUnlockingIngredients() const
    {
        return unlockingIngredients;
    }

    /** @throws DataDecodeException */
    static RecipeUnlockingRequirement read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<std::vector<RecipeIngredient>> unlockingIngredients;
};

}  // namespace bedrock_protocol::types::recipe
