/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/InventoryTransactionPacket.php
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
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/inventory/InventoryTransactionChangedSlotsHack.h"
#include "bedrock_protocol/protocol/types/inventory/MismatchTransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/NormalTransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/ReleaseItemTransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/UseItemOnEntityTransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/UseItemTransactionData.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

/**
 * This packet effectively crams multiple packets into one.
 */
class InventoryTransactionPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::INVENTORY_TRANSACTION_PACKET;

    static constexpr std::int32_t TYPE_NORMAL = 0;
    static constexpr std::int32_t TYPE_MISMATCH = 1;
    static constexpr std::int32_t TYPE_USE_ITEM = 2;
    static constexpr std::int32_t TYPE_USE_ITEM_ON_ENTITY = 3;
    static constexpr std::int32_t TYPE_RELEASE_ITEM = 4;

    std::int32_t requestId = 0;
    /** @var InventoryTransactionChangedSlotsHack[] */
    std::optional<std::vector<types::inventory::InventoryTransactionChangedSlotsHack>> requestChangedSlots;
    std::unique_ptr<types::inventory::TransactionData> trData;

    /**
     * @generate-create-func
     */
    static InventoryTransactionPacket create(std::int32_t requestId, std::optional<std::vector<types::inventory::InventoryTransactionChangedSlotsHack>> requestChangedSlots, std::unique_ptr<types::inventory::TransactionData> trData);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "InventoryTransactionPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
