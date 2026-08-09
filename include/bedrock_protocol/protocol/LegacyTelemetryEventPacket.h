/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LegacyTelemetryEventPacket.php
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

class LegacyTelemetryEventPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::LEGACY_TELEMETRY_EVENT_PACKET;

    static constexpr std::int32_t TYPE_ACHIEVEMENT_AWARDED = 0;
    static constexpr std::int32_t TYPE_ENTITY_INTERACT = 1;
    static constexpr std::int32_t TYPE_PORTAL_BUILT = 2;
    static constexpr std::int32_t TYPE_PORTAL_USED = 3;
    static constexpr std::int32_t TYPE_MOB_KILLED = 4;
    static constexpr std::int32_t TYPE_CAULDRON_USED = 5;
    static constexpr std::int32_t TYPE_PLAYER_DEATH = 6;
    static constexpr std::int32_t TYPE_BOSS_KILLED = 7;
    static constexpr std::int32_t TYPE_AGENT_COMMAND = 8;
    static constexpr std::int32_t TYPE_AGENT_CREATED = 9;
    static constexpr std::int32_t TYPE_PATTERN_REMOVED = 10;  //???
    static constexpr std::int32_t TYPE_COMMANED_EXECUTED = 11;
    static constexpr std::int32_t TYPE_FISH_BUCKETED = 12;
    static constexpr std::int32_t TYPE_MOB_BORN = 13;
    static constexpr std::int32_t TYPE_PET_DIED = 14;
    static constexpr std::int32_t TYPE_CAULDRON_BLOCK_USED = 15;
    static constexpr std::int32_t TYPE_COMPOSTER_BLOCK_USED = 16;
    static constexpr std::int32_t TYPE_BELL_BLOCK_USED = 17;
    static constexpr std::int32_t TYPE_ACTOR_DEFINITION = 18;
    static constexpr std::int32_t TYPE_RAID_UPDATE = 19;
    static constexpr std::int32_t TYPE_PLAYER_MOVEMENT_ANOMALY = 20;  //anti cheat
    static constexpr std::int32_t TYPE_PLAYER_MOVEMENT_CORRECTED = 21;
    static constexpr std::int32_t TYPE_HONEY_HARVESTED = 22;
    static constexpr std::int32_t TYPE_TARGET_BLOCK_HIT = 23;
    static constexpr std::int32_t TYPE_PIGLIN_BARTER = 24;

    std::uint64_t playerRuntimeId = 0;
    std::int32_t eventData = 0;
    std::uint8_t type = 0;

    /**
     * The per-type body, kept verbatim because nothing here knows how to read it.
     *
     * PHP stops after `type` with a comment calling the rest a confusing mess, and this port inherited
     * that. A real 1.26.40 server does send a body: captures carry 7 and 13 further bytes depending on
     * the event, so decoding stopped early and re-encoding dropped everything after `type`.
     *
     * Holding the remainder as opaque bytes makes the round trip lossless without inventing a layout
     * for the two dozen event types. Anything that needs a field out of this has to parse it against
     * `type` itself.
     */
    std::string extraData;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "LegacyTelemetryEventPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
