/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetPlayerInventoryOptionsPacket.php
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
#include "bedrock_protocol/protocol/types/inventory/InventoryLayout.h"
#include "bedrock_protocol/protocol/types/inventory/InventoryLeftTab.h"
#include "bedrock_protocol/protocol/types/inventory/InventoryRightTab.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class SetPlayerInventoryOptionsPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_PLAYER_INVENTORY_OPTIONS_PACKET;


    types::inventory::InventoryLeftTab leftTab = types::inventory::InventoryLeftTab::NONE;
    types::inventory::InventoryRightTab rightTab = types::inventory::InventoryRightTab::NONE;
    bool filtering = false;
    types::inventory::InventoryLayout inventoryLayout = types::inventory::InventoryLayout::NONE;
    types::inventory::InventoryLayout craftingLayout = types::inventory::InventoryLayout::NONE;

    /**
     * @generate-create-func
     */
    static SetPlayerInventoryOptionsPacket create(types::inventory::InventoryLeftTab leftTab, types::inventory::InventoryRightTab rightTab, bool filtering, types::inventory::InventoryLayout inventoryLayout, types::inventory::InventoryLayout craftingLayout);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetPlayerInventoryOptionsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
