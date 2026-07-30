/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/ItemDescriptor.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::recipe {

/**
 * Describes what items are accepted in a recipe input.
 */
class ItemDescriptor {
public:
    virtual ~ItemDescriptor() = default;

    [[nodiscard]] virtual std::uint8_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for polymorphic members. */
    [[nodiscard]] virtual std::unique_ptr<ItemDescriptor> clone() const = 0;

    /**
     * Reads the descriptor body for the given descriptor type ID.
     *
     * The PHP original performs this dispatch inline in CommonTypes::getRecipeIngredient(); it maps an
     * unrecognised type ID to null rather than throwing, and this port preserves that behaviour, because
     * type ID 0 ("no descriptor") is a legitimate wire value.
     *
     * @throws DataDecodeException
     */
    static std::unique_ptr<ItemDescriptor> read(encoding::ByteBufferReader &in, std::uint8_t descriptorType);

protected:
    ItemDescriptor() = default;
    ItemDescriptor(const ItemDescriptor &) = default;
    ItemDescriptor &operator=(const ItemDescriptor &) = default;
};

}  // namespace bedrock_protocol::types::recipe
