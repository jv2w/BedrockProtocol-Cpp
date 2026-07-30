/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/CreativeGroupEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"

namespace bedrock_protocol::types::inventory {

class CreativeGroupEntry final {
public:
    CreativeGroupEntry(std::int32_t categoryId, std::string categoryName, ItemStack icon)
        : categoryId(categoryId), categoryName(std::move(categoryName)), icon(std::move(icon))
    {
    }

    [[nodiscard]] std::int32_t getCategoryId() const { return categoryId; }

    [[nodiscard]] const std::string &getCategoryName() const { return categoryName; }

    [[nodiscard]] const ItemStack &getIcon() const { return icon; }

    static CreativeGroupEntry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t categoryId;
    std::string categoryName;
    ItemStack icon;
};

}  // namespace bedrock_protocol::types::inventory
