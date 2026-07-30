/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/ItemStackWrapper.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>

#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"

namespace bedrock_protocol::types::inventory {

class ItemStackWrapper final {
public:
    ItemStackWrapper(std::int32_t stackId, ItemStack itemStack, std::uint32_t stackIdVariant = 0)
        : stackId(stackId), itemStack(std::move(itemStack)), stackIdVariant(stackIdVariant)
    {
    }

    static ItemStackWrapper legacy(ItemStack itemStack)
    {
        const std::int32_t stackId = itemStack.getId() == 0 ? 0 : 1;
        return {stackId, std::move(itemStack)};
    }

    [[nodiscard]] std::int32_t getStackId() const { return stackId; }

    [[nodiscard]] std::uint32_t getStackIdVariant() const { return stackIdVariant; }

    [[nodiscard]] const ItemStack &getItemStack() const { return itemStack; }

private:
    std::int32_t stackId;
    ItemStack itemStack;
    std::uint32_t stackIdVariant;
};

}  // namespace bedrock_protocol::types::inventory
