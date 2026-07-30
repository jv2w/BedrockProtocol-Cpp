/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetSpawnPositionPacket.php
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

class SetSpawnPositionPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_SPAWN_POSITION_PACKET;

    static constexpr std::int32_t TYPE_PLAYER_SPAWN = 0;
    static constexpr std::int32_t TYPE_WORLD_SPAWN = 1;

    std::int32_t spawnType = 0;
    types::BlockPosition spawnPosition;
    std::int32_t dimension = 0;
    /**
     * Position of the respawn anchor or bed that this spawn position was set by.
     * This may be different from the spawn position (e.g. the actual spawn position may be next to a bed, while this
     * would be the position of the bed block itself).
     */
    types::BlockPosition causingBlockPosition;

    /**
     * @generate-create-func
     */
    static SetSpawnPositionPacket create(std::int32_t spawnType, types::BlockPosition spawnPosition, std::int32_t dimension, types::BlockPosition causingBlockPosition);

    static SetSpawnPositionPacket playerSpawn(types::BlockPosition spawnPosition, std::int32_t dimension, types::BlockPosition causingBlockPosition);

    static SetSpawnPositionPacket worldSpawn(types::BlockPosition spawnPosition, std::int32_t dimension);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetSpawnPositionPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
