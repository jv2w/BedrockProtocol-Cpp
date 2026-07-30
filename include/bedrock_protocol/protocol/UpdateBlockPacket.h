/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateBlockPacket.php
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
#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class UpdateBlockPacket : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_BLOCK_PACKET;

    static constexpr std::int32_t FLAG_NONE = 0b0000;
    static constexpr std::int32_t FLAG_NEIGHBORS = 0b0001;
    static constexpr std::int32_t FLAG_NETWORK = 0b0010;
    static constexpr std::int32_t FLAG_NOGRAPHIC = 0b0100;
    static constexpr std::int32_t FLAG_PRIORITY = 0b1000;
    static constexpr std::int32_t DATA_LAYER_NORMAL = 0;
    static constexpr std::int32_t DATA_LAYER_LIQUID = 1;

    types::BlockPosition blockPosition;
    std::uint32_t blockRuntimeId = 0;
    /**
     * Flags are used by MCPE internally for block setting, but only flag 2 (network flag) is relevant for network.
     * This field is pointless really.
     */
    std::uint32_t flags = FLAG_NETWORK;
    std::uint32_t dataLayerId = DATA_LAYER_NORMAL;

    /**
     * @generate-create-func
     */
    static UpdateBlockPacket create(types::BlockPosition blockPosition, std::uint32_t blockRuntimeId, std::uint32_t flags, std::uint32_t dataLayerId);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdateBlockPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
