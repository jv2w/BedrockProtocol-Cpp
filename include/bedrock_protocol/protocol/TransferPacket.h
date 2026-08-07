/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/TransferPacket.php
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
#include "bedrock_protocol/protocol/types/GatheringJoinInfo.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class TransferPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::TRANSFER_PACKET;


    std::string address;
    std::uint16_t port = 19132;
    bool reloadWorld = false;
    std::optional<types::GatheringJoinInfo> gatheringJoinInfo = std::nullopt;

    /**
     * @generate-create-func
     */
    static TransferPacket create(std::string address, std::uint16_t port, bool reloadWorld, std::optional<types::GatheringJoinInfo> gatheringJoinInfo);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "TransferPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
