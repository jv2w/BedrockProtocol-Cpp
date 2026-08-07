/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerAuthInputPacket.php
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
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequest.h"
#include "bedrock_protocol/protocol/types/ItemInteractionData.h"
#include "bedrock_protocol/protocol/types/PlayerAuthInputFlagList.h"
#include "bedrock_protocol/protocol/types/PlayerAuthInputFlags.h"
#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class PlayerAuthInputPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PLAYER_AUTH_INPUT_PACKET;


    math::Vector3 position;
    float pitch = 0.0F;
    float yaw = 0.0F;
    float headYaw = 0.0F;
    float moveVecX = 0.0F;
    float moveVecZ = 0.0F;
    types::PlayerAuthInputFlagList inputFlags;
    std::uint32_t inputMode = 0;
    std::uint32_t playMode = 0;
    std::int32_t interactionMode = 0;
    math::Vector2 interactRotation;
    std::uint64_t tick = 0;
    math::Vector3 delta;
    std::optional<types::ItemInteractionData> itemInteractionData = std::nullopt;
    std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest = std::nullopt;
    /** @var PlayerBlockAction[]|null */
    std::optional<std::vector<types::PlayerBlockAction>> blockActions = std::nullopt;
    std::optional<math::Vector2> vehicleRotation = std::nullopt;
    std::optional<std::int64_t> clientPredictedVehicle = std::nullopt;
    float analogMoveVecX = 0.0F;
    float analogMoveVecZ = 0.0F;
    math::Vector3 cameraOrientation;
    math::Vector2 rawMove;

    /**
     * @param inputFlags @see PlayerAuthInputFlags
     * @param inputMode @see InputMode
     * @param playMode @see PlayMode
     * @param interactionMode @see InteractionMode
     * @param blockActions Blocks that the client has interacted with
     */
    static PlayerAuthInputPacket create(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, types::PlayerAuthInputFlagList inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::int32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<types::PlayerBlockAction>> blockActions, std::optional<math::Vector2> vehicleRotation, std::optional<std::int64_t> clientPredictedVehicle, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PlayerAuthInputPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    /**
     * @generate-create-func
     * @param blockActions
     */
    static PlayerAuthInputPacket internalCreate(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, types::PlayerAuthInputFlagList inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::int32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<types::PlayerBlockAction>> blockActions, std::optional<math::Vector2> vehicleRotation, std::optional<std::int64_t> clientPredictedVehicle, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove);

    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
