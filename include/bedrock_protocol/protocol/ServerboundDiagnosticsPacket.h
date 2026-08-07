/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ServerboundDiagnosticsPacket.php
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
#include "bedrock_protocol/protocol/types/EntityDiagnosticTimingInfo.h"
#include "bedrock_protocol/protocol/types/MemoryCategoryCounter.h"
#include "bedrock_protocol/protocol/types/SystemCategory.h"
#include "bedrock_protocol/protocol/types/SystemDiagnosticTimingInfo.h"
#include "bedrock_protocol/protocol/types/WhiskerScopeDataSummary.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ServerboundDiagnosticsPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SERVERBOUND_DIAGNOSTICS_PACKET;


    float avgFps = 0.0F;
    float avgServerSimTickTimeMS = 0.0F;
    float avgClientSimTickTimeMS = 0.0F;
    float avgBeginFrameTimeMS = 0.0F;
    float avgInputTimeMS = 0.0F;
    float avgRenderTimeMS = 0.0F;
    float avgEndFrameTimeMS = 0.0F;
    float avgRemainderTimePercent = 0.0F;
    float avgUnaccountedTimePercent = 0.0F;
    /**
     */
    std::vector<types::MemoryCategoryCounter> memoryCategoryValues;
    /**
     */
    std::vector<types::EntityDiagnosticTimingInfo> entityDiagnostics;
    /**
     */
    std::vector<types::SystemDiagnosticTimingInfo> systemDiagnostics;
    /**
     */
    std::vector<types::SystemCategory> systemCategories;
    /**
     */
    std::vector<types::WhiskerScopeDataSummary> whiskerScopes;

    /**
     * @generate-create-func
     */
    static ServerboundDiagnosticsPacket create(float avgFps, float avgServerSimTickTimeMS, float avgClientSimTickTimeMS, float avgBeginFrameTimeMS, float avgInputTimeMS, float avgRenderTimeMS, float avgEndFrameTimeMS, float avgRemainderTimePercent, float avgUnaccountedTimePercent, std::vector<types::MemoryCategoryCounter> memoryCategoryValues, std::vector<types::EntityDiagnosticTimingInfo> entityDiagnostics, std::vector<types::SystemDiagnosticTimingInfo> systemDiagnostics, std::vector<types::SystemCategory> systemCategories, std::vector<types::WhiskerScopeDataSummary> whiskerScopes);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ServerboundDiagnosticsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
