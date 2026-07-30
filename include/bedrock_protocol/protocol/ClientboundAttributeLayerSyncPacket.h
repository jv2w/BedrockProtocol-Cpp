/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundAttributeLayerSyncPacket.php
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
#include "bedrock_protocol/protocol/types/AttributeLayerSyncPayload.h"
#include "bedrock_protocol/protocol/types/AttributesRemoveEnvironment.h"
#include "bedrock_protocol/protocol/types/AttributesUpdateEnvironment.h"
#include "bedrock_protocol/protocol/types/AttributeUpdateLayers.h"
#include "bedrock_protocol/protocol/types/AttributeUpdateLayerSettings.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientboundAttributeLayerSyncPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENTBOUND_ATTRIBUTE_LAYER_SYNC_PACKET;


    std::unique_ptr<types::AttributeLayerSyncPayload> payload;

    /**
     * @generate-create-func
     */
    static ClientboundAttributeLayerSyncPacket create(std::unique_ptr<types::AttributeLayerSyncPayload> payload);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientboundAttributeLayerSyncPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
