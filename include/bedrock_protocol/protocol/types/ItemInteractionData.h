/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ItemInteractionData.php
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
#include "bedrock_protocol/protocol/types/inventory/InventoryTransactionChangedSlotsHack.h"
#include "bedrock_protocol/protocol/types/inventory/UseItemTransactionData.h"

namespace bedrock_protocol::types {

class ItemInteractionData final {
public:
    ItemInteractionData(std::int32_t requestId,
                        std::vector<inventory::InventoryTransactionChangedSlotsHack> requestChangedSlots,
                        inventory::UseItemTransactionData transactionData)
        : requestId(requestId), requestChangedSlots(std::move(requestChangedSlots)),
          transactionData(std::move(transactionData))
    {
    }

    [[nodiscard]] std::int32_t getRequestId() const { return requestId; }

    [[nodiscard]] const std::vector<inventory::InventoryTransactionChangedSlotsHack> &getRequestChangedSlots() const
    {
        return requestChangedSlots;
    }

    [[nodiscard]] const inventory::UseItemTransactionData &getTransactionData() const { return transactionData; }

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static ItemInteractionData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t requestId;
    std::vector<inventory::InventoryTransactionChangedSlotsHack> requestChangedSlots;
    inventory::UseItemTransactionData transactionData;
};

}  // namespace bedrock_protocol::types
