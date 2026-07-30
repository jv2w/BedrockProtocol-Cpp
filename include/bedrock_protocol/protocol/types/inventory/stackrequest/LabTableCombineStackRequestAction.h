/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/LabTableCombineStackRequestAction.php
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
 * Not clear what the point of this is. It's sent when the player uses a lab table, but it's not clear why this action
 * is needed.
 */
class LabTableCombineStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::LAB_TABLE_COMBINE;

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    static LabTableCombineStackRequestAction read(encoding::ByteBufferReader &in) {
        (void)in;
        return LabTableCombineStackRequestAction();
    }

    void write(encoding::ByteBufferWriter &out) const override {
        (void)out;
        //NOOP
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<LabTableCombineStackRequestAction>(*this);
    }
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
