/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/TransactionData.php
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

namespace bedrock_protocol::types::inventory {

class TransactionData {
public:
    virtual ~TransactionData() = default;

    [[nodiscard]] const std::vector<NetworkInventoryAction> &getActions() const { return actions; }

    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for polymorphic members. */
    [[nodiscard]] virtual std::unique_ptr<TransactionData> clone() const = 0;

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    void decodeTransaction(encoding::ByteBufferReader &in);

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    void decodeAuthInput(encoding::ByteBufferReader &in);

    void encodeTransaction(encoding::ByteBufferWriter &out) const;

    void encodeAuthInput(encoding::ByteBufferWriter &out) const;

protected:
    TransactionData() = default;
    TransactionData(const TransactionData &) = default;
    TransactionData &operator=(const TransactionData &) = default;

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    virtual void decodeData(encoding::ByteBufferReader &in) = 0;

    virtual void encodeData(encoding::ByteBufferWriter &out) const = 0;

    std::vector<NetworkInventoryAction> actions;
};

}  // namespace bedrock_protocol::types::inventory
