/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AddPlayerPacket.php
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
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/uuid/Uuid.h"
#include "bedrock_protocol/protocol/types/DeviceOS.h"
#include "bedrock_protocol/protocol/types/entity/EntityLink.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/PropertySyncData.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"
#include "bedrock_protocol/protocol/UpdateAbilitiesPacket.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class AddPlayerPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::ADD_PLAYER_PACKET;


    uuid::Uuid uuid;
    std::string username;
    std::uint64_t actorRuntimeId = 0;
    std::string platformChatId = "";
    math::Vector3 position;
    std::optional<math::Vector3> motion = std::nullopt;
    float pitch = 0.0;
    float yaw = 0.0;
    float headYaw = 0.0;
    types::inventory::ItemStackWrapper item{0, types::inventory::ItemStack::null()};
    std::int32_t gameMode = 0;
    /**
     * @note array<int, MetadataProperty>
     */
    serializer::CommonTypes::EntityMetadata metadata;
    types::entity::PropertySyncData syncedProperties{{}, {}};
    UpdateAbilitiesPacket abilitiesPacket;
    /** @var EntityLink[] */
    std::vector<types::entity::EntityLink> links;
    std::string deviceId = "";  //TODO: fill player's device ID (???)
    std::int32_t buildPlatform = types::DeviceOS::UNKNOWN;

    /**
     * @generate-create-func
     */
    static AddPlayerPacket create(uuid::Uuid uuid, std::string username, std::uint64_t actorRuntimeId, std::string platformChatId, math::Vector3 position, std::optional<math::Vector3> motion, float pitch, float yaw, float headYaw, types::inventory::ItemStackWrapper item, std::int32_t gameMode, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, UpdateAbilitiesPacket abilitiesPacket, std::vector<types::entity::EntityLink> links, std::string deviceId, std::int32_t buildPlatform);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AddPlayerPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
