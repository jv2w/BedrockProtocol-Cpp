/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/UpdateEquipPacket.php
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

namespace bedrock_protocol {

class PacketHandlerInterface;

class UpdateEquipPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_EQUIP_PACKET;


    std::uint8_t windowId = 0;
    std::uint8_t windowType = 0;
    std::int32_t windowSlotCount = 0;  //useless, seems to be part of a standard container header
    std::int64_t actorUniqueId = 0;
    /** @phpstan-var CacheableNbt<\pocketmine\nbt\tag\CompoundTag> */
    types::CacheableNbt<nbt::tag::CompoundTag> nbt;

    /**
     * @generate-create-func
     */
    static UpdateEquipPacket create(std::uint8_t windowId, std::uint8_t windowType, std::int32_t windowSlotCount, std::int64_t actorUniqueId, types::CacheableNbt<nbt::tag::CompoundTag> nbt);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdateEquipPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
