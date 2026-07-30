/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/MineBlockStackRequestAction.php
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
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

class MineBlockStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::MINE_BLOCK;

    MineBlockStackRequestAction(std::int32_t hotbarSlot, std::int32_t predictedDurability, std::int32_t stackId) :
        hotbarSlot(hotbarSlot), predictedDurability(predictedDurability), stackId(stackId) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::int32_t getHotbarSlot() const { return hotbarSlot; }

    [[nodiscard]] std::int32_t getPredictedDurability() const { return predictedDurability; }

    [[nodiscard]] std::int32_t getStackId() const { return stackId; }

    static MineBlockStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto hotbarSlot = encoding::VarInt::readSignedInt(in);
        const auto predictedDurability = encoding::VarInt::readSignedInt(in);
        const auto stackId = serializer::CommonTypes::readItemStackNetIdVariant(in);
        return MineBlockStackRequestAction(hotbarSlot, predictedDurability, stackId);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::VarInt::writeSignedInt(out, hotbarSlot);
        encoding::VarInt::writeSignedInt(out, predictedDurability);
        serializer::CommonTypes::writeItemStackNetIdVariant(out, stackId);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<MineBlockStackRequestAction>(*this);
    }

private:
    std::int32_t hotbarSlot;
    std::int32_t predictedDurability;
    std::int32_t stackId;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
