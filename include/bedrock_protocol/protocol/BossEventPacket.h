/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/BossEventPacket.php
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
#include "bedrock_protocol/protocol/types/BossBarColor.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class BossEventPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::BOSS_EVENT_PACKET;

    /** S2C: Shows the boss-bar to the player. */
    static constexpr std::int32_t TYPE_SHOW = 0;
    /** C2S: Registers a player to a boss fight. */
    static constexpr std::int32_t TYPE_REGISTER_PLAYER = 1;
    /** S2C: Removes the boss-bar from the client. */
    static constexpr std::int32_t TYPE_HIDE = 2;
    /** C2S: Unregisters a player from a boss fight. */
    static constexpr std::int32_t TYPE_UNREGISTER_PLAYER = 3;
    /** S2C: Sets the bar percentage. */
    static constexpr std::int32_t TYPE_HEALTH_PERCENT = 4;
    /** S2C: Sets title of the bar. */
    static constexpr std::int32_t TYPE_TITLE = 5;
    /** S2C: Updates misc properties of the bar and environment. */
    static constexpr std::int32_t TYPE_PROPERTIES = 6;
    /** S2C: Updates boss-bar colour and overlay texture. */
    static constexpr std::int32_t TYPE_TEXTURE = 7;
    /** C2S: Client asking the server to resend all boss data. */
    static constexpr std::int32_t TYPE_QUERY = 8;

    std::int64_t bossActorUniqueId = 0;
    std::uint8_t eventType = 0;
    std::int64_t playerActorUniqueId = 0;
    float healthPercent = 0.0;
    std::string title = "";
    std::string filteredTitle = "";
    std::uint8_t color = types::BossBarColor::PINK;
    std::uint8_t overlay = 0;

    static BossEventPacket show(std::int64_t bossActorUniqueId, std::string title, float healthPercent, std::uint8_t color = types::BossBarColor::PURPLE, std::uint8_t overlay = 0);

    static BossEventPacket hide(std::int64_t bossActorUniqueId);

    static BossEventPacket registerPlayer(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId);

    static BossEventPacket unregisterPlayer(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId);

    /** @note PHP calls this healthPercent(); renamed because C++ cannot have a method and a field of the same name. */
    static BossEventPacket makeHealthPercent(std::int64_t bossActorUniqueId, float healthPercent);

    /** @note PHP calls this title(); renamed because C++ cannot have a method and a field of the same name. */
    static BossEventPacket makeTitle(std::int64_t bossActorUniqueId, std::string title);

    static BossEventPacket properties(std::int64_t bossActorUniqueId, std::uint8_t color = types::BossBarColor::PURPLE, std::uint8_t overlay = 0);

    static BossEventPacket query(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "BossEventPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    static BossEventPacket base(std::int64_t bossActorUniqueId, std::int32_t eventId);

};

}  // namespace bedrock_protocol
