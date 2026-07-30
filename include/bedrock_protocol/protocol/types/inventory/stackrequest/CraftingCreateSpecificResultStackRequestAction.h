/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CraftingCreateSpecificResultStackRequestAction.php
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

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * This action precedes a "take" or "place" action involving the "created item" magic slot. It indicates that the
 * "created item" output slot now contains output N of a previously specified crafting recipe.
 * This is only used with crafting recipes that have multiple outputs. For recipes with single outputs, it's assumed
 * that the content of the "created item" slot is the only output.
 *
 * @see ContainerUIIds::CREATED_OUTPUT
 * @see UIInventorySlotOffset::CREATED_ITEM_OUTPUT
 */
class CraftingCreateSpecificResultStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_CREATE_SPECIFIC_RESULT;

    explicit CraftingCreateSpecificResultStackRequestAction(std::uint8_t resultIndex) : resultIndex(resultIndex) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint8_t getResultIndex() const { return resultIndex; }

    static CraftingCreateSpecificResultStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto slot = encoding::Byte::readUnsigned(in);
        return CraftingCreateSpecificResultStackRequestAction(slot);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::Byte::writeUnsigned(out, resultIndex);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<CraftingCreateSpecificResultStackRequestAction>(*this);
    }

private:
    std::uint8_t resultIndex;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
