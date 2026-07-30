/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/CreativeItemEntry.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"

namespace bedrock_protocol::types::inventory {

class CreativeItemEntry final {
public:
    CreativeItemEntry(std::uint32_t entryId, ItemStack item, std::uint32_t groupId)
        : entryId(entryId), item(std::move(item)), groupId(groupId)
    {
    }

    [[nodiscard]] std::uint32_t getEntryId() const { return entryId; }

    [[nodiscard]] const ItemStack &getItem() const { return item; }

    [[nodiscard]] std::uint32_t getGroupId() const { return groupId; }

    static CreativeItemEntry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t entryId;
    ItemStack item;
    std::uint32_t groupId;
};

}  // namespace bedrock_protocol::types::inventory
