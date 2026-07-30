/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponseSlotInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::inventory::stackresponse {

class ItemStackResponseSlotInfo final {
public:
    ItemStackResponseSlotInfo(std::uint8_t slot, std::uint8_t hotbarSlot, std::uint8_t count, std::int32_t itemStackId,
                              std::string customName, std::string filteredCustomName,
                              std::int32_t durabilityCorrection)
        : slot(slot), hotbarSlot(hotbarSlot), count(count), itemStackId(itemStackId),
          customName(std::move(customName)), filteredCustomName(std::move(filteredCustomName)),
          durabilityCorrection(durabilityCorrection)
    {
    }

    [[nodiscard]] std::uint8_t getSlot() const { return slot; }

    [[nodiscard]] std::uint8_t getHotbarSlot() const { return hotbarSlot; }

    [[nodiscard]] std::uint8_t getCount() const { return count; }

    [[nodiscard]] std::int32_t getItemStackId() const { return itemStackId; }

    [[nodiscard]] const std::string &getCustomName() const { return customName; }

    [[nodiscard]] const std::string &getFilteredCustomName() const { return filteredCustomName; }

    [[nodiscard]] std::int32_t getDurabilityCorrection() const { return durabilityCorrection; }

    static ItemStackResponseSlotInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t slot;
    std::uint8_t hotbarSlot;
    std::uint8_t count;
    std::int32_t itemStackId;
    std::string customName;
    std::string filteredCustomName;
    std::int32_t durabilityCorrection;
};

}  // namespace bedrock_protocol::types::inventory::stackresponse
