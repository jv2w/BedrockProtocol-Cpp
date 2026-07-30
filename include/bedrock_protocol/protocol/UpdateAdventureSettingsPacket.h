/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/UpdateAdventureSettingsPacket.php
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
 * Updates "adventure settings". In vanilla, these flags apply to the whole world. This differs from abilities, which
 * apply only to the local player itself.
 * In practice, there's no difference between the two for a custom server.
 * This includes flags such as worldImmutable (makes players unable to build), autoJump, showNameTags, noPvM, and noMvP.
 */
class UpdateAdventureSettingsPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_ADVENTURE_SETTINGS_PACKET;


    bool noAttackingMobs = false;
    bool noAttackingPlayers = false;
    bool worldImmutable = false;
    bool showNameTags = false;
    bool autoJump = false;

    /**
     * @generate-create-func
     */
    static UpdateAdventureSettingsPacket create(bool noAttackingMobs, bool noAttackingPlayers, bool worldImmutable, bool showNameTags, bool autoJump);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdateAdventureSettingsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
