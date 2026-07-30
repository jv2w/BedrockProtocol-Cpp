/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdatePlayerGameTypePacket.php
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

namespace bedrock_protocol {

class PacketHandlerInterface;

class UpdatePlayerGameTypePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_PLAYER_GAME_TYPE_PACKET;


    /** @see GameMode */
    std::int32_t gameMode = 0;
    std::int64_t playerActorUniqueId = 0;
    std::uint64_t tick = 0;

    /**
     * @generate-create-func
     */
    static UpdatePlayerGameTypePacket create(std::int32_t gameMode, std::int64_t playerActorUniqueId, std::uint64_t tick);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdatePlayerGameTypePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
