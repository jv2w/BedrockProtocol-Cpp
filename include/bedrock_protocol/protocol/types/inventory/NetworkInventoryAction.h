/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/NetworkInventoryAction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"

namespace bedrock_protocol::types::inventory {

class NetworkInventoryAction {
public:
    static constexpr std::uint32_t SOURCE_CONTAINER = 0;

    static constexpr std::uint32_t SOURCE_GLOBAL = 1;
    static constexpr std::uint32_t SOURCE_WORLD = 2;  //drop/pickup item entity
    static constexpr std::uint32_t SOURCE_CREATIVE = 3;
    static constexpr std::uint32_t SOURCE_TODO = 99999;

    /**
     * Fake window IDs for the SOURCE_TODO type (99999)
     *
     * These identifiers are used for inventory source types which are not currently implemented server-side in MCPE.
     * As a general rule of thumb, anything that doesn't have a permanent inventory is client-side. These types are
     * to allow servers to track what is going on in client-side windows.
     *
     * Expect these to change in the future.
     */
    static constexpr std::int32_t SOURCE_TYPE_CRAFTING_RESULT = -4;
    static constexpr std::int32_t SOURCE_TYPE_CRAFTING_USE_INGREDIENT = -5;

    static constexpr std::int32_t SOURCE_TYPE_ANVIL_RESULT = -12;
    static constexpr std::int32_t SOURCE_TYPE_ANVIL_OUTPUT = -13;

    static constexpr std::int32_t SOURCE_TYPE_ENCHANT_OUTPUT = -17;

    static constexpr std::int32_t SOURCE_TYPE_TRADING_INPUT_1 = -20;
    static constexpr std::int32_t SOURCE_TYPE_TRADING_INPUT_2 = -21;
    static constexpr std::int32_t SOURCE_TYPE_TRADING_USE_INPUTS = -22;
    static constexpr std::int32_t SOURCE_TYPE_TRADING_OUTPUT = -23;

    static constexpr std::int32_t SOURCE_TYPE_BEACON = -24;

    static constexpr std::int32_t ACTION_MAGIC_SLOT_CREATIVE_DELETE_ITEM = 0;
    static constexpr std::int32_t ACTION_MAGIC_SLOT_CREATIVE_CREATE_ITEM = 1;

    static constexpr std::int32_t ACTION_MAGIC_SLOT_DROP_ITEM = 0;
    static constexpr std::int32_t ACTION_MAGIC_SLOT_PICKUP_ITEM = 1;

    std::uint32_t sourceType = 0;
    std::optional<std::int32_t> windowId;
    std::optional<std::uint32_t> sourceFlags;
    std::uint32_t inventorySlot = 0;
    ItemStackWrapper oldItem{0, ItemStack::null()};
    ItemStackWrapper newItem{0, ItemStack::null()};

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    NetworkInventoryAction &readAuthInput(encoding::ByteBufferReader &in);

    void writeAuthInput(encoding::ByteBufferWriter &out) const;

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    NetworkInventoryAction &readTransaction(encoding::ByteBufferReader &in);

    /**
     * @throws std::invalid_argument
     */
    void writeTransaction(encoding::ByteBufferWriter &out) const;
};

}  // namespace bedrock_protocol::types::inventory
