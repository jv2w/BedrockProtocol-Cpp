/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/DestroyStackRequestAction.php
 * (the DisappearStackRequestActionTrait members are inlined here, per the porting rules for PHP traits)
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

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Sends some (or all) items from the source slot to /dev/null. This happens when the player clicks items into the
 * creative inventory menu in creative mode.
 */
class DestroyStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::DESTROY;

    DestroyStackRequestAction(std::uint8_t count, ItemStackRequestSlotInfo source) :
        count(count), source(std::move(source)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint8_t getCount() const { return count; }

    [[nodiscard]] const ItemStackRequestSlotInfo &getSource() const { return source; }

    static DestroyStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto count = encoding::Byte::readUnsigned(in);
        auto source = ItemStackRequestSlotInfo::read(in);
        return DestroyStackRequestAction(count, std::move(source));
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::Byte::writeUnsigned(out, count);
        source.write(out);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<DestroyStackRequestAction>(*this);
    }

private:
    std::uint8_t count;
    ItemStackRequestSlotInfo source;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
