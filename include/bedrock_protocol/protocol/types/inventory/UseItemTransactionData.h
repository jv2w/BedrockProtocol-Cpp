/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/UseItemTransactionData.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"
#include "bedrock_protocol/protocol/types/inventory/NetworkInventoryAction.h"
#include "bedrock_protocol/protocol/types/inventory/PredictedResult.h"
#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/TriggerType.h"

namespace bedrock_protocol::types::inventory {

class UseItemTransactionData : public TransactionData {
public:
    /**
     * Deviation from PHP: this is InventoryTransactionPacket::TYPE_USE_ITEM, but the packet header includes
     * this one, so the value is bound out-of-line in the .cpp to break the include cycle. It is therefore
     * not usable as a switch-case label.
     */
    static const std::int32_t ID;

    static constexpr std::int32_t ACTION_CLICK_BLOCK = 0;
    static constexpr std::int32_t ACTION_CLICK_AIR = 1;
    static constexpr std::int32_t ACTION_BREAK_BLOCK = 2;
    static constexpr std::int32_t ACTION_USE_AS_ATTACK = 3;

    UseItemTransactionData() = default;

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::unique_ptr<TransactionData> clone() const override
    {
        return std::make_unique<UseItemTransactionData>(*this);
    }

    [[nodiscard]] std::int32_t getActionType() const { return actionType; }

    [[nodiscard]] TriggerType getTriggerType() const { return triggerType; }

    [[nodiscard]] const types::BlockPosition &getBlockPosition() const { return blockPosition; }

    [[nodiscard]] std::uint8_t getFace() const { return face; }

    [[nodiscard]] std::int32_t getHotbarSlot() const { return hotbarSlot; }

    [[nodiscard]] const ItemStackWrapper &getItemInHand() const { return itemInHand; }

    [[nodiscard]] const math::Vector3 &getPlayerPosition() const { return playerPosition; }

    [[nodiscard]] const math::Vector3 &getClickPosition() const { return clickPosition; }

    [[nodiscard]] std::uint32_t getBlockRuntimeId() const { return blockRuntimeId; }

    [[nodiscard]] PredictedResult getClientInteractPrediction() const { return clientInteractPrediction; }

    [[nodiscard]] std::uint8_t getClientCooldownState() const { return clientCooldownState; }

    /**
     * Renamed from PHP `new()`; `new` is a reserved word in C++.
     */
    static UseItemTransactionData create(std::vector<NetworkInventoryAction> actions, std::int32_t actionType,
                                         TriggerType triggerType, types::BlockPosition blockPosition, std::uint8_t face,
                                         std::int32_t hotbarSlot, ItemStackWrapper itemInHand,
                                         math::Vector3 playerPosition, math::Vector3 clickPosition,
                                         std::uint32_t blockRuntimeId, PredictedResult clientInteractPrediction,
                                         std::uint8_t clientCooldownState);

protected:
    void decodeData(encoding::ByteBufferReader &in) override;

    void encodeData(encoding::ByteBufferWriter &out) const override;

private:
    /**
     * @generate-create-func
     */
    static UseItemTransactionData initSelf(std::int32_t actionType, TriggerType triggerType,
                                           types::BlockPosition blockPosition, std::uint8_t face,
                                           std::int32_t hotbarSlot, ItemStackWrapper itemInHand,
                                           math::Vector3 playerPosition, math::Vector3 clickPosition,
                                           std::uint32_t blockRuntimeId, PredictedResult clientInteractPrediction,
                                           std::uint8_t clientCooldownState);

    std::int32_t actionType = 0;
    TriggerType triggerType = TriggerType::UNKNOWN;
    types::BlockPosition blockPosition{0, 0, 0};
    std::uint8_t face = 0;
    std::int32_t hotbarSlot = 0;
    ItemStackWrapper itemInHand{0, ItemStack::null()};
    math::Vector3 playerPosition;
    math::Vector3 clickPosition;
    std::uint32_t blockRuntimeId = 0;
    PredictedResult clientInteractPrediction = PredictedResult::FAILURE;
    std::uint8_t clientCooldownState = 0;
};

}  // namespace bedrock_protocol::types::inventory
