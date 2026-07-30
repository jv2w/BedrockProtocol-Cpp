/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CreativeCreateStackRequestAction.php
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
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Creates an item by copying it from the creative inventory. This is treated as a crafting action by vanilla.
 */
class CreativeCreateStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CREATIVE_CREATE;

    CreativeCreateStackRequestAction(std::uint32_t creativeItemId, std::uint8_t repetitions) :
        creativeItemId(creativeItemId), repetitions(repetitions) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint32_t getCreativeItemId() const { return creativeItemId; }

    [[nodiscard]] std::uint8_t getRepetitions() const { return repetitions; }

    static CreativeCreateStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto creativeItemId = serializer::CommonTypes::readCreativeItemNetId(in);
        const auto repetitions = encoding::Byte::readUnsigned(in);
        return CreativeCreateStackRequestAction(creativeItemId, repetitions);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        serializer::CommonTypes::writeCreativeItemNetId(out, creativeItemId);
        encoding::Byte::writeUnsigned(out, repetitions);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<CreativeCreateStackRequestAction>(*this);
    }

private:
    std::uint32_t creativeItemId;
    std::uint8_t repetitions;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
