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

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/recipe/RecipeIngredient.h"

namespace bedrock_protocol::types::recipe {

class RecipeUnlockingRequirement final {
public:
    //gophertunnel minecraft/protocol/recipe.go:59-64
    static constexpr std::int32_t CONTEXT_NONE = 0;
    static constexpr std::int32_t CONTEXT_ALWAYS_UNLOCKED = 1;
    static constexpr std::int32_t CONTEXT_PLAYER_IN_WATER = 2;
    static constexpr std::int32_t CONTEXT_PLAYER_HAS_MANY_ITEMS = 3;

    RecipeUnlockingRequirement(std::int32_t context, std::vector<RecipeIngredient> unlockingIngredients)
        : context(context), unlockingIngredients(std::move(unlockingIngredients))
    {
    }

    [[nodiscard]] std::int32_t getContext() const { return context; }

    /** Only meaningful when the context is CONTEXT_NONE; nothing else is sent over the wire. */
    [[nodiscard]] const std::vector<RecipeIngredient> &getUnlockingIngredients() const
    {
        return unlockingIngredients;
    }

    /** @throws DataDecodeException */
    static RecipeUnlockingRequirement read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t context;
    std::vector<RecipeIngredient> unlockingIngredients;
};

}  // namespace bedrock_protocol::types::recipe
