/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ItemStack.php
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

namespace bedrock_protocol::types::inventory {

class ItemStack final {
public:
    /**
     * @param rawExtraData Serialized ItemStackExtraData (use ItemStackExtraData->write())
     * @see ItemStackExtraData::write()
     */
    ItemStack(std::int32_t id, std::uint32_t meta, std::uint16_t count, std::int32_t blockRuntimeId,
              std::string rawExtraData)
        : id(id), meta(meta), count(count), blockRuntimeId(blockRuntimeId), rawExtraData(std::move(rawExtraData))
    {
    }

    static ItemStack null() { return {0, 0, 0, 0, ""}; }

    [[nodiscard]] bool isNull() const { return id == 0; }

    [[nodiscard]] std::int32_t getId() const { return id; }

    [[nodiscard]] std::uint32_t getMeta() const { return meta; }

    [[nodiscard]] std::uint16_t getCount() const { return count; }

    [[nodiscard]] std::int32_t getBlockRuntimeId() const { return blockRuntimeId; }

    /**
     * Decode this into ItemStackExtraData using ItemStackExtraData::read() (or ItemStackExtraDataShield::read() if this
     * data is for a shield item)
     * This isn't automatically decoded because it's usually not needed and is sometimes expensive to decode.
     * @see ItemStackExtraData::read()
     * @see ItemStackExtraDataShield::read()
     */
    [[nodiscard]] const std::string &getRawExtraData() const { return rawExtraData; }

    [[nodiscard]] bool equals(const ItemStack &itemStack) const
    {
        return equalsWithoutCount(itemStack) && count == itemStack.count;
    }

    [[nodiscard]] bool equalsWithoutCount(const ItemStack &itemStack) const
    {
        return id == itemStack.id && meta == itemStack.meta && blockRuntimeId == itemStack.blockRuntimeId &&
               rawExtraData == itemStack.rawExtraData;
    }

    friend bool operator==(const ItemStack &a, const ItemStack &b) { return a.equals(b); }
    friend bool operator!=(const ItemStack &a, const ItemStack &b) { return !a.equals(b); }

    /**
     * @note PHP implements \JsonSerializable here (jsonSerialize(), base64-encoding rawExtraData).
     *       Intentionally omitted: this port has no JSON facility, and nothing inside BedrockProtocol
     *       calls it - it exists for PHP userland debugging only.
     */

private:
    std::int32_t id;
    std::uint32_t meta;
    std::uint16_t count;
    std::int32_t blockRuntimeId;
    std::string rawExtraData;
};

}  // namespace bedrock_protocol::types::inventory
