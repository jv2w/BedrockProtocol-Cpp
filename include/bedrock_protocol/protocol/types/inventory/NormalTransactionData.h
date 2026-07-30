/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/NormalTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/NetworkInventoryAction.h"
#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"

namespace bedrock_protocol::types::inventory {

class NormalTransactionData : public TransactionData {
public:
    /**
     * Deviation from PHP: this is InventoryTransactionPacket::TYPE_NORMAL, but the packet header includes
     * this one, so the value is bound out-of-line in the .cpp to break the include cycle. It is therefore
     * not usable as a switch-case label.
     */
    static const std::int32_t ID;

    NormalTransactionData() = default;

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::unique_ptr<TransactionData> clone() const override
    {
        return std::make_unique<NormalTransactionData>(*this);
    }

    /**
     * Renamed from PHP `new()`; `new` is a reserved word in C++.
     */
    static NormalTransactionData create(std::vector<NetworkInventoryAction> actions);

protected:
    void decodeData(encoding::ByteBufferReader &in) override;

    void encodeData(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol::types::inventory
