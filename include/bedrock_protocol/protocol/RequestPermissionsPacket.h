/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RequestPermissionsPacket.php
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

/**
 * Sent by the client to request that the server change permissions of a player. This could be itself or another player.
 * Used when toggling permission switches or changing a player's permission level in the pause menu.
 */
class RequestPermissionsPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::REQUEST_PERMISSIONS_PACKET;

    static constexpr std::int32_t FLAG_BUILD = 1 << 0;
    static constexpr std::int32_t FLAG_MINE = 1 << 1;
    static constexpr std::int32_t FLAG_DOORS_AND_SWITCHES = 1 << 2;
    static constexpr std::int32_t FLAG_OPEN_CONTAINERS = 1 << 3;
    static constexpr std::int32_t FLAG_ATTACK_PLAYERS = 1 << 4;
    static constexpr std::int32_t FLAG_ATTACK_MOBS = 1 << 5;
    static constexpr std::int32_t FLAG_OPERATOR = 1 << 6;
    static constexpr std::int32_t FLAG_TELEPORT = 1 << 7;

    std::int64_t targetActorUniqueId = 0;
    /** @see PlayerPermissions */
    std::int32_t playerPermission = 0;
    std::uint16_t customFlags = 0;

    /**
     * @generate-create-func
     */
    static RequestPermissionsPacket create(std::int64_t targetActorUniqueId, std::int32_t playerPermission, std::uint16_t customFlags);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "RequestPermissionsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
