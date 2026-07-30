/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateTradePacket.php
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
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"
#include "bedrock_protocol/protocol/types/inventory/WindowTypes.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class UpdateTradePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_TRADE_PACKET;


    std::uint8_t windowId = 0;
    std::uint8_t windowType = types::inventory::WindowTypes::TRADING;  //Mojang hardcoded this -_-
    std::int32_t windowSlotCount = 0;  //useless, seems to be part of a standard container header
    std::int32_t tradeTier = 0;
    std::int64_t traderActorUniqueId = 0;
    std::int64_t playerActorUniqueId = 0;
    std::string displayName;
    bool isV2Trading = false;
    bool isEconomyTrading = false;
    /** @phpstan-var CacheableNbt<\pocketmine\nbt\tag\CompoundTag> */
    types::CacheableNbt<nbt::tag::CompoundTag> offers;

    /**
     * @generate-create-func
     */
    static UpdateTradePacket create(std::uint8_t windowId, std::uint8_t windowType, std::int32_t windowSlotCount, std::int32_t tradeTier, std::int64_t traderActorUniqueId, std::int64_t playerActorUniqueId, std::string displayName, bool isV2Trading, bool isEconomyTrading, types::CacheableNbt<nbt::tag::CompoundTag> offers);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdateTradePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
