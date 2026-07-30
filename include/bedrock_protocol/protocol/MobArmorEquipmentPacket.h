/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MobArmorEquipmentPacket.php
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
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class MobArmorEquipmentPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MOB_ARMOR_EQUIPMENT_PACKET;


    std::uint64_t actorRuntimeId = 0;
    //this intentionally doesn't use an array because we don't want any implicit dependencies on internal order
    types::inventory::ItemStackWrapper head{0, types::inventory::ItemStack::null()};
    types::inventory::ItemStackWrapper chest{0, types::inventory::ItemStack::null()};
    types::inventory::ItemStackWrapper legs{0, types::inventory::ItemStack::null()};
    types::inventory::ItemStackWrapper feet{0, types::inventory::ItemStack::null()};
    types::inventory::ItemStackWrapper body{0, types::inventory::ItemStack::null()};

    /**
     * @generate-create-func
     */
    static MobArmorEquipmentPacket create(std::uint64_t actorRuntimeId, types::inventory::ItemStackWrapper head, types::inventory::ItemStackWrapper chest, types::inventory::ItemStackWrapper legs, types::inventory::ItemStackWrapper feet, types::inventory::ItemStackWrapper body);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MobArmorEquipmentPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
