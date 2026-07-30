/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/RecipeIngredient.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <utility>

#include "bedrock_protocol/protocol/types/recipe/ItemDescriptor.h"

namespace bedrock_protocol::types::recipe {

class RecipeIngredient final {
public:
    RecipeIngredient(std::unique_ptr<ItemDescriptor> descriptor, std::int32_t count)
        : descriptor(std::move(descriptor)), count(count)
    {
    }

    /** Not present in the PHP original; deep-copies the polymorphic descriptor to preserve value semantics. */
    RecipeIngredient(const RecipeIngredient &other)
        : descriptor(other.descriptor != nullptr ? other.descriptor->clone() : nullptr), count(other.count)
    {
    }

    RecipeIngredient &operator=(const RecipeIngredient &other)
    {
        if (this != &other) {
            descriptor = other.descriptor != nullptr ? other.descriptor->clone() : nullptr;
            count = other.count;
        }
        return *this;
    }

    RecipeIngredient(RecipeIngredient &&) noexcept = default;
    RecipeIngredient &operator=(RecipeIngredient &&) noexcept = default;
    ~RecipeIngredient() = default;

    [[nodiscard]] const ItemDescriptor *getDescriptor() const { return descriptor.get(); }

    [[nodiscard]] std::int32_t getCount() const { return count; }

private:
    std::unique_ptr<ItemDescriptor> descriptor;
    std::int32_t count;
};

}  // namespace bedrock_protocol::types::recipe
