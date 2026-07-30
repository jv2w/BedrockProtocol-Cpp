/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponseContainerInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/FullContainerName.h"
#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.h"

namespace bedrock_protocol::types::inventory::stackresponse {

class ItemStackResponseContainerInfo final {
public:
    /**
     * @note slots contains ItemStackResponseSlotInfo elements
     */
    ItemStackResponseContainerInfo(FullContainerName containerName, std::vector<ItemStackResponseSlotInfo> slots)
        : containerName(std::move(containerName)), slots(std::move(slots))
    {
    }

    [[nodiscard]] const FullContainerName &getContainerName() const { return containerName; }

    [[nodiscard]] const std::vector<ItemStackResponseSlotInfo> &getSlots() const { return slots; }

    static ItemStackResponseContainerInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    FullContainerName containerName;
    std::vector<ItemStackResponseSlotInfo> slots;
};

}  // namespace bedrock_protocol::types::inventory::stackresponse
