/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/RecipeWithTypeId.php
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

#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::recipe {

class RecipeWithTypeId {
public:
    virtual ~RecipeWithTypeId() = default;

    [[nodiscard]] std::int32_t getTypeId() const { return typeId; }

    virtual void encode(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for polymorphic members. */
    [[nodiscard]] virtual std::unique_ptr<RecipeWithTypeId> clone() const = 0;

protected:
    explicit RecipeWithTypeId(std::int32_t typeId) : typeId(typeId) {}

    RecipeWithTypeId(const RecipeWithTypeId &) = default;
    RecipeWithTypeId &operator=(const RecipeWithTypeId &) = default;

private:
    std::int32_t typeId;
};

}  // namespace bedrock_protocol::types::recipe
