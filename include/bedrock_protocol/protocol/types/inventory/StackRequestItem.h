/*
 * This file is part of BedrockProtocol-Cpp.
 * No PHP counterpart: this type was introduced by the 1.26.40 item redesign.
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

/**
 * The descriptor-backed item format used by the deprecated craft-result stack request action. Unlike an
 * ItemStack it identifies the item by name rather than by a numeric network ID.
 *
 * Mirror of gophertunnel minecraft/protocol/item.go:41-60.
 */
class StackRequestItem final {
public:
    StackRequestItem(std::string identifier, std::int32_t meta, std::int32_t blockRuntimeId, std::uint16_t count,
                     std::string rawExtraData)
        : identifier(std::move(identifier)), meta(meta), blockRuntimeId(blockRuntimeId), count(count),
          rawExtraData(std::move(rawExtraData))
    {
    }

    static StackRequestItem null() { return {"", 0, 0, 0, ""}; }

    [[nodiscard]] const std::string &getIdentifier() const { return identifier; }

    [[nodiscard]] std::int32_t getMeta() const { return meta; }

    [[nodiscard]] std::int32_t getBlockRuntimeId() const { return blockRuntimeId; }

    [[nodiscard]] std::uint16_t getCount() const { return count; }

    /** @see ItemStack::getRawExtraData() */
    [[nodiscard]] const std::string &getRawExtraData() const { return rawExtraData; }

private:
    std::string identifier;
    std::int32_t meta;
    std::int32_t blockRuntimeId;
    std::uint16_t count;
    std::string rawExtraData;
};

}  // namespace bedrock_protocol::types::inventory
