/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/EducationSettingsPacket.php
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
#include "bedrock_protocol/protocol/types/EducationSettingsAgentCapabilities.h"
#include "bedrock_protocol/protocol/types/EducationSettingsExternalLinkSettings.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class EducationSettingsPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::EDUCATION_SETTINGS_PACKET;


    std::string codeBuilderDefaultUri;
    std::string codeBuilderTitle;
    bool canResizeCodeBuilder = false;
    bool disableLegacyTitleBar = false;
    std::string postProcessFilter;
    std::string screenshotBorderResourcePath;
    std::optional<types::EducationSettingsAgentCapabilities> agentCapabilities;
    std::optional<std::string> codeBuilderOverrideUri;
    bool hasQuiz = false;
    std::optional<types::EducationSettingsExternalLinkSettings> linkSettings;

    /**
     * @generate-create-func
     */
    static EducationSettingsPacket create(std::string codeBuilderDefaultUri, std::string codeBuilderTitle, bool canResizeCodeBuilder, bool disableLegacyTitleBar, std::string postProcessFilter, std::string screenshotBorderResourcePath, std::optional<types::EducationSettingsAgentCapabilities> agentCapabilities, std::optional<std::string> codeBuilderOverrideUri, bool hasQuiz, std::optional<types::EducationSettingsExternalLinkSettings> linkSettings);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "EducationSettingsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
