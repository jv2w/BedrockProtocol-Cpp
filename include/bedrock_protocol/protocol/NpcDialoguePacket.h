/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/NpcDialoguePacket.php
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

class NpcDialoguePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::NPC_DIALOGUE_PACKET;

    static constexpr std::int32_t ACTION_OPEN = 0;
    static constexpr std::int32_t ACTION_CLOSE = 1;

    std::int64_t npcActorUniqueId = 0;
    std::int32_t actionType = 0;
    std::string dialogue;
    std::string sceneName;
    std::string npcName;
    std::string actionJson;

    /**
     * @generate-create-func
     */
    static NpcDialoguePacket create(std::int64_t npcActorUniqueId, std::int32_t actionType, std::string dialogue, std::string sceneName, std::string npcName, std::string actionJson);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "NpcDialoguePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
