/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/ItemStackRequestSlotInfo.php
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

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/types/inventory/FullContainerName.h"

namespace bedrock_protocol::types::inventory::stackrequest {

class ItemStackRequestSlotInfo final {
public:
    ItemStackRequestSlotInfo(inventory::FullContainerName containerName, std::uint8_t slotId, std::int32_t stackId) :
        containerName(std::move(containerName)), slotId(slotId), stackId(stackId) {}

    [[nodiscard]] const inventory::FullContainerName &getContainerName() const { return containerName; }

    [[nodiscard]] std::uint8_t getSlotId() const { return slotId; }

    [[nodiscard]] std::int32_t getStackId() const { return stackId; }

    static ItemStackRequestSlotInfo read(encoding::ByteBufferReader &in) {
        auto containerName = inventory::FullContainerName::read(in);
        const auto slotId = encoding::Byte::readUnsigned(in);
        //gophertunnel minecraft/protocol/item_stack.go:622-626 - a fixed little-endian int32, not a varint.
        const auto stackId = encoding::LE::readSignedInt(in);
        return ItemStackRequestSlotInfo(std::move(containerName), slotId, stackId);
    }

    void write(encoding::ByteBufferWriter &out) const {
        containerName.write(out);
        encoding::Byte::writeUnsigned(out, slotId);
        encoding::LE::writeSignedInt(out, stackId);
    }

private:
    inventory::FullContainerName containerName;
    std::uint8_t slotId;
    std::int32_t stackId;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
