/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AddItemActorPacket.php
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
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class AddItemActorPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::ADD_ITEM_ACTOR_PACKET;


    std::int64_t actorUniqueId = 0;
    std::uint64_t actorRuntimeId = 0;
    std::optional<types::inventory::ItemStackWrapper> item;  //@note PHP leaves this uninitialised until decode/create; C++ needs std::optional because the type has no default constructor
    math::Vector3 position;
    std::optional<math::Vector3> motion = std::nullopt;
    /**
     * @note keyed by metadata property id
     */
    serializer::CommonTypes::EntityMetadata metadata;
    bool isFromFishing = false;

    /**
     * @generate-create-func
     */
    static AddItemActorPacket create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, types::inventory::ItemStackWrapper item, math::Vector3 position, std::optional<math::Vector3> motion, serializer::CommonTypes::EntityMetadata metadata, bool isFromFishing);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AddItemActorPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
