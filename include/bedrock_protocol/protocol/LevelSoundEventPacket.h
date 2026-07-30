/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LevelSoundEventPacket.php
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
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class LevelSoundEventPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::LEVEL_SOUND_EVENT_PACKET;


    /** @see LevelSoundEvent */
    std::string sound;
    math::Vector3 position;
    std::int32_t extraData = -1;
    std::string entityType = ":";  //???
    bool isBabyMob = false;  //...
    bool disableRelativeVolume = false;
    std::int64_t actorUniqueId = -1;
    std::optional<math::Vector3> firePosition = std::nullopt;

    /**
     * @generate-create-func
     */
    static LevelSoundEventPacket create(std::string sound, math::Vector3 position, std::int32_t extraData, std::string entityType, bool isBabyMob, bool disableRelativeVolume, std::int64_t actorUniqueId, std::optional<math::Vector3> firePosition);

    static LevelSoundEventPacket nonActorSound(std::string sound, math::Vector3 position, bool disableRelativeVolume, std::int32_t extraData = -1);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "LevelSoundEventPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
