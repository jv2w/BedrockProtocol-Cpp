/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/DimensionDataPacket.php
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
#include <utility>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/DimensionData.h"
#include "bedrock_protocol/protocol/types/DimensionNameIds.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

/**
 * Sets properties of different dimensions of the world, such as its Y axis bounds and generator used
 */
class DimensionDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::DIMENSION_DATA_PACKET;


    /**
     * @note Keyed by a DimensionNameIds constant; the encode order is the insertion order.
     */
    std::vector<std::pair<std::string, types::DimensionData>> definitions;

    /**
     * @generate-create-func
     */
    static DimensionDataPacket create(std::vector<std::pair<std::string, types::DimensionData>> definitions);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "DimensionDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
