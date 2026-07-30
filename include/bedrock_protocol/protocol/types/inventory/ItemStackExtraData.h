/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ItemStackExtraData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"

namespace bedrock_protocol::types::inventory {

/**
 * Wrapper class for extra data on ItemStacks.
 * The data is normally provided as a raw string (not automatically decoded).
 * This class is just a DTO for PacketSerializer to use when encoding/decoding ItemStacks.
 */
class ItemStackExtraData {
public:
    ItemStackExtraData(std::optional<::bedrock_protocol::nbt::tag::CompoundTag> nbt, std::vector<std::string> canPlaceOn,
                       std::vector<std::string> canDestroy)
        : nbt(std::move(nbt)), canPlaceOn(std::move(canPlaceOn)), canDestroy(std::move(canDestroy))
    {
    }

    virtual ~ItemStackExtraData() = default;

    ItemStackExtraData(const ItemStackExtraData &) = default;
    ItemStackExtraData &operator=(const ItemStackExtraData &) = default;
    ItemStackExtraData(ItemStackExtraData &&) = default;
    ItemStackExtraData &operator=(ItemStackExtraData &&) = default;

    [[nodiscard]] const std::vector<std::string> &getCanPlaceOn() const { return canPlaceOn; }

    [[nodiscard]] const std::vector<std::string> &getCanDestroy() const { return canDestroy; }

    [[nodiscard]] const std::optional<::bedrock_protocol::nbt::tag::CompoundTag> &getNbt() const { return nbt; }

    /**
     * @throws PacketDecodeException
     */
    static ItemStackExtraData read(encoding::ByteBufferReader &in);

    virtual void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<::bedrock_protocol::nbt::tag::CompoundTag> nbt;
    std::vector<std::string> canPlaceOn;
    std::vector<std::string> canDestroy;
};

}  // namespace bedrock_protocol::types::inventory
