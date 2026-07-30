/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/StructureTemplateDataRequestPacket.php
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
#include "bedrock_protocol/protocol/types/StructureSettings.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class StructureTemplateDataRequestPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::STRUCTURE_TEMPLATE_DATA_REQUEST_PACKET;

    static constexpr std::int32_t TYPE_EXPORT_FROM_SAVE_MODE = 1;
    static constexpr std::int32_t TYPE_EXPORT_FROM_LOAD_MODE = 2;
    static constexpr std::int32_t TYPE_QUERY_SAVED_STRUCTURE = 3;

    std::string structureTemplateName;
    types::BlockPosition structureBlockPosition;
    types::StructureSettings structureSettings;
    std::uint8_t requestType = 0;

    /**
     * @generate-create-func
     */
    static StructureTemplateDataRequestPacket create(std::string structureTemplateName, types::BlockPosition structureBlockPosition, types::StructureSettings structureSettings, std::uint8_t requestType);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "StructureTemplateDataRequestPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
