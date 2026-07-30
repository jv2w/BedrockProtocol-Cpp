/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/InventoryTransactionChangedSlotsHack.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::inventory {

class InventoryTransactionChangedSlotsHack final {
public:
    InventoryTransactionChangedSlotsHack(std::uint8_t containerId, std::vector<std::uint8_t> changedSlotIndexes)
        : containerId(containerId), changedSlotIndexes(std::move(changedSlotIndexes))
    {
    }

    [[nodiscard]] std::uint8_t getContainerId() const { return containerId; }

    [[nodiscard]] const std::vector<std::uint8_t> &getChangedSlotIndexes() const { return changedSlotIndexes; }

    static InventoryTransactionChangedSlotsHack read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t containerId;
    std::vector<std::uint8_t> changedSlotIndexes;
};

}  // namespace bedrock_protocol::types::inventory
