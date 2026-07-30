/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AddActorPacket.php
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
#include "bedrock_protocol/protocol/types/entity/Attribute.h"
#include "bedrock_protocol/protocol/types/entity/EntityLink.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/PropertySyncData.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class AddActorPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::ADD_ACTOR_PACKET;


    std::int64_t actorUniqueId = 0;
    std::uint64_t actorRuntimeId = 0;
    std::string type;
    math::Vector3 position;
    std::optional<math::Vector3> motion = std::nullopt;
    float pitch = 0.0;
    float yaw = 0.0;
    float headYaw = 0.0;
    float bodyYaw = 0.0;  //???
    /** @var Attribute[] */
    std::vector<types::entity::Attribute> attributes;
    /**
     */
    serializer::CommonTypes::EntityMetadata metadata;
    types::entity::PropertySyncData syncedProperties{{}, {}};
    /** @var EntityLink[] */
    std::vector<types::entity::EntityLink> links;

    /**
     * @generate-create-func
     */
    static AddActorPacket create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, std::string type, math::Vector3 position, std::optional<math::Vector3> motion, float pitch, float yaw, float headYaw, float bodyYaw, std::vector<types::entity::Attribute> attributes, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, std::vector<types::entity::EntityLink> links);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AddActorPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
