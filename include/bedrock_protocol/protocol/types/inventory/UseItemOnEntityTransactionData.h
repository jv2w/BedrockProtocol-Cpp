/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/UseItemOnEntityTransactionData.php
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
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"
#include "bedrock_protocol/protocol/types/inventory/NetworkInventoryAction.h"
#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"

namespace bedrock_protocol::types::inventory {

class UseItemOnEntityTransactionData : public TransactionData {
public:
    /**
     * Deviation from PHP: this is InventoryTransactionPacket::TYPE_USE_ITEM_ON_ENTITY, but the packet header
     * includes this one, so the value is bound out-of-line in the .cpp to break the include cycle. It is
     * therefore not usable as a switch-case label.
     */
    static const std::int32_t ID;

    static constexpr std::int32_t ACTION_INTERACT = 0;
    static constexpr std::int32_t ACTION_ATTACK = 1;
    static constexpr std::int32_t ACTION_ITEM_INTERACT = 2;

    UseItemOnEntityTransactionData() = default;

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::unique_ptr<TransactionData> clone() const override
    {
        return std::make_unique<UseItemOnEntityTransactionData>(*this);
    }

    [[nodiscard]] std::uint64_t getActorRuntimeId() const { return actorRuntimeId; }

    [[nodiscard]] std::int32_t getActionType() const { return actionType; }

    [[nodiscard]] std::int32_t getHotbarSlot() const { return hotbarSlot; }

    [[nodiscard]] const ItemStackWrapper &getItemInHand() const { return itemInHand; }

    [[nodiscard]] const math::Vector3 &getPlayerPosition() const { return playerPosition; }

    [[nodiscard]] const math::Vector3 &getClickPosition() const { return clickPosition; }

    /**
     * Renamed from PHP `new()`; `new` is a reserved word in C++.
     */
    static UseItemOnEntityTransactionData create(std::vector<NetworkInventoryAction> actions,
                                                 std::uint64_t actorRuntimeId, std::int32_t actionType,
                                                 std::int32_t hotbarSlot, ItemStackWrapper itemInHand,
                                                 math::Vector3 playerPosition, math::Vector3 clickPosition);

protected:
    void decodeData(encoding::ByteBufferReader &in) override;

    void encodeData(encoding::ByteBufferWriter &out) const override;

private:
    /**
     * @generate-create-func
     */
    static UseItemOnEntityTransactionData initSelf(std::uint64_t actorRuntimeId, std::int32_t actionType,
                                                   std::int32_t hotbarSlot, ItemStackWrapper itemInHand,
                                                   math::Vector3 playerPosition, math::Vector3 clickPosition);

    std::uint64_t actorRuntimeId = 0;
    std::int32_t actionType = 0;
    std::int32_t hotbarSlot = 0;
    ItemStackWrapper itemInHand{0, ItemStack::null()};
    math::Vector3 playerPosition;
    math::Vector3 clickPosition;
};

}  // namespace bedrock_protocol::types::inventory
