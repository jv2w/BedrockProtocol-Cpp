/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ItemStackExtraDataShield.php
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
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackExtraData.h"

namespace bedrock_protocol::types::inventory {

/**
 * Extension of ItemStackExtraData for shield items, which have an additional field for the blocking tick.
 */
class ItemStackExtraDataShield final : public ItemStackExtraData {
public:
    ItemStackExtraDataShield(std::optional<::bedrock_protocol::nbt::tag::CompoundTag> nbt,
                             std::vector<std::string> canPlaceOn, std::vector<std::string> canDestroy,
                             std::int64_t blockingTick)
        : ItemStackExtraData(std::move(nbt), std::move(canPlaceOn), std::move(canDestroy)), blockingTick(blockingTick)
    {
    }

    [[nodiscard]] std::int64_t getBlockingTick() const { return blockingTick; }

    /**
     * @throws PacketDecodeException
     */
    static ItemStackExtraDataShield read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

private:
    std::int64_t blockingTick;
};

}  // namespace bedrock_protocol::types::inventory
