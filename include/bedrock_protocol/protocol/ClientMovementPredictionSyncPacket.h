/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientMovementPredictionSyncPacket.php
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
#include "bedrock_protocol/protocol/serializer/BitSet.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientMovementPredictionSyncPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENT_MOVEMENT_PREDICTION_SYNC_PACKET;

    /**
     * Deliberate divergence from PHP, which uses EntityMetadataFlags::NUMBER_OF_FLAGS (131) here.
     *
     * A BitSet is written as a fixed ceil(length / 7) bytes, so the length is visible on the wire.
     * A 1.26.40 client sends this bitset as 16 bytes, which puts the length between 106 and 112; the
     * highest bit it was seen to set is 109, which narrows it to 110..112. 131 would be 19 bytes, so
     * re-encoding a captured packet produced three bytes of padding the client never sent.
     *
     * The three surviving candidates are indistinguishable on the wire. 112 is chosen because it is
     * the largest of them: if the real count is lower, the extra indices are simply never set, while
     * choosing too low would reject a flag the client does use.
     *
     * The maintained PHP fork agrees with upstream on 131, so both references are wrong together
     * here; only the captured bytes settle it.
     */
    static constexpr std::int32_t FLAG_LENGTH = 112;

    serializer::BitSet flags;
    float scale = 0.0F;
    float width = 0.0F;
    float height = 0.0F;
    float movementSpeed = 0.0F;
    float underwaterMovementSpeed = 0.0F;
    float lavaMovementSpeed = 0.0F;
    float jumpStrength = 0.0F;
    float health = 0.0F;
    float hunger = 0.0F;
    float frictionModifier = 0.0F;
    float bounciness = 0.0F;
    float airDragModifier = 0.0F;
    std::int64_t actorUniqueId = 0;
    bool actorFlyingState = false;

    /**
     * @generate-create-func
     */
    static ClientMovementPredictionSyncPacket internalCreate(serializer::BitSet flags, float scale, float width, float height, float movementSpeed, float underwaterMovementSpeed, float lavaMovementSpeed, float jumpStrength, float health, float hunger, float frictionModifier, float bounciness, float airDragModifier, std::int64_t actorUniqueId, bool actorFlyingState);

    static ClientMovementPredictionSyncPacket create(serializer::BitSet flags, float scale, float width, float height, float movementSpeed, float underwaterMovementSpeed, float lavaMovementSpeed, float jumpStrength, float health, float hunger, float frictionModifier, float bounciness, float airDragModifier, std::int64_t actorUniqueId, bool actorFlyingState);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientMovementPredictionSyncPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
