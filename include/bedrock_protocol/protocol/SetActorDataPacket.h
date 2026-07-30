/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetActorDataPacket.php
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
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/PropertySyncData.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class SetActorDataPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_ACTOR_DATA_PACKET;


    std::uint64_t actorRuntimeId = 0;
    /**
     * @note array<int, MetadataProperty>
     */
    serializer::CommonTypes::EntityMetadata metadata;
    types::entity::PropertySyncData syncedProperties{{}, {}};
    std::uint64_t tick = 0;

    /**
     * @generate-create-func
     */
    static SetActorDataPacket create(std::uint64_t actorRuntimeId, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, std::uint64_t tick);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetActorDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
