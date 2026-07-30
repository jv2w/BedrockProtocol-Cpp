/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/DropStackRequestAction.php
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
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Drops some (or all) items from the source slot into the world as an item entity.
 */
class DropStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::DROP;

    DropStackRequestAction(std::uint8_t count, ItemStackRequestSlotInfo source, bool randomly) :
        count(count), source(std::move(source)), randomly(randomly) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint8_t getCount() const { return count; }

    [[nodiscard]] const ItemStackRequestSlotInfo &getSource() const { return source; }

    [[nodiscard]] bool isRandomly() const { return randomly; }

    static DropStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto count = encoding::Byte::readUnsigned(in);
        auto source = ItemStackRequestSlotInfo::read(in);
        const auto random = serializer::CommonTypes::getBool(in);
        return DropStackRequestAction(count, std::move(source), random);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::Byte::writeUnsigned(out, count);
        source.write(out);
        serializer::CommonTypes::putBool(out, randomly);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<DropStackRequestAction>(*this);
    }

private:
    std::uint8_t count;
    ItemStackRequestSlotInfo source;
    bool randomly;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
