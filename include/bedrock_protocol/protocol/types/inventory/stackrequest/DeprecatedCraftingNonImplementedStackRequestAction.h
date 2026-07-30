/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/DeprecatedCraftingNonImplementedStackRequestAction.php
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
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Tells that the current transaction involves crafting an item in a way that isn't supported by the current system.
 * At the time of writing, this includes using anvils.
 */
class DeprecatedCraftingNonImplementedStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_NON_IMPLEMENTED_DEPRECATED_ASK_TY_LAING;

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    static DeprecatedCraftingNonImplementedStackRequestAction read(encoding::ByteBufferReader &in) {
        (void)in;
        return DeprecatedCraftingNonImplementedStackRequestAction();
    }

    void write(encoding::ByteBufferWriter &out) const override {
        (void)out;
        //NOOP
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<DeprecatedCraftingNonImplementedStackRequestAction>(*this);
    }
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
