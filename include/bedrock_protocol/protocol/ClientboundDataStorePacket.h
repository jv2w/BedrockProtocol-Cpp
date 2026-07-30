/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundDataStorePacket.php
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
#include "bedrock_protocol/protocol/types/ddui/DataStoreChange.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreOperation.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreOperationType.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreRemoval.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreUpdate.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientboundDataStorePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENTBOUND_DATA_STORE_PACKET;


    /**
     * @note list<DataStoreOperation>
     */
    std::vector<std::unique_ptr<types::ddui::DataStoreOperation>> values;

    /**
     * @generate-create-func
     */
    static ClientboundDataStorePacket create(std::vector<std::unique_ptr<types::ddui::DataStoreOperation>> values);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientboundDataStorePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
