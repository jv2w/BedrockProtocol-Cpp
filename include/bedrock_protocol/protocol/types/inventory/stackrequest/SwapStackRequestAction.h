/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/SwapStackRequestAction.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Swaps two stacks. These don't have to be in the same inventory. This action does not modify the stacks themselves.
 */
class SwapStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::SWAP;

    SwapStackRequestAction(ItemStackRequestSlotInfo slot1, ItemStackRequestSlotInfo slot2) :
        slot1(std::move(slot1)), slot2(std::move(slot2)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const ItemStackRequestSlotInfo &getSlot1() const { return slot1; }

    [[nodiscard]] const ItemStackRequestSlotInfo &getSlot2() const { return slot2; }

    static SwapStackRequestAction read(encoding::ByteBufferReader &in) {
        auto slot1 = ItemStackRequestSlotInfo::read(in);
        auto slot2 = ItemStackRequestSlotInfo::read(in);
        return SwapStackRequestAction(std::move(slot1), std::move(slot2));
    }

    void write(encoding::ByteBufferWriter &out) const override {
        slot1.write(out);
        slot2.write(out);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<SwapStackRequestAction>(*this);
    }

private:
    ItemStackRequestSlotInfo slot1;
    ItemStackRequestSlotInfo slot2;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
