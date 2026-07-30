/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/GraphicsOverrideParameterPacket.php
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
#include "bedrock_protocol/protocol/types/GraphicsOverrideParameterType.h"
#include "bedrock_protocol/protocol/types/ParameterKeyframeValue.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class GraphicsOverrideParameterPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::GRAPHICS_OVERRIDE_PARAMETER_PACKET;


    /** @var ParameterKeyframeValue[] */
    std::vector<types::ParameterKeyframeValue> values;
    std::optional<float> unknownFloat;
    std::optional<math::Vector3> unknownVector3;
    std::string biomeIdentifier;
    std::optional<std::string> playerIdentifier;
    types::GraphicsOverrideParameterType parameterType = types::GraphicsOverrideParameterType::SKY_ZENITH_COLOR;
    bool reset = false;

    /**
     * @generate-create-func
     */
    static GraphicsOverrideParameterPacket create(std::vector<types::ParameterKeyframeValue> values, std::optional<float> unknownFloat, std::optional<math::Vector3> unknownVector3, std::string biomeIdentifier, std::optional<std::string> playerIdentifier, types::GraphicsOverrideParameterType parameterType, bool reset);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "GraphicsOverrideParameterPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
