/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/EducationSettingsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/EducationSettingsPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

EducationSettingsPacket EducationSettingsPacket::create(std::string codeBuilderDefaultUri, std::string codeBuilderTitle, bool canResizeCodeBuilder, bool disableLegacyTitleBar, std::string postProcessFilter, std::string screenshotBorderResourcePath, std::optional<types::EducationSettingsAgentCapabilities> agentCapabilities, std::optional<std::string> codeBuilderOverrideUri, bool hasQuiz, std::optional<types::EducationSettingsExternalLinkSettings> linkSettings)
{
    EducationSettingsPacket result;
    result.codeBuilderDefaultUri = std::move(codeBuilderDefaultUri);
    result.codeBuilderTitle = std::move(codeBuilderTitle);
    result.canResizeCodeBuilder = canResizeCodeBuilder;
    result.disableLegacyTitleBar = disableLegacyTitleBar;
    result.postProcessFilter = std::move(postProcessFilter);
    result.screenshotBorderResourcePath = std::move(screenshotBorderResourcePath);
    result.agentCapabilities = std::move(agentCapabilities);
    result.codeBuilderOverrideUri = std::move(codeBuilderOverrideUri);
    result.hasQuiz = hasQuiz;
    result.linkSettings = std::move(linkSettings);
    return result;
}

void EducationSettingsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    codeBuilderDefaultUri = serializer::CommonTypes::getString(in);
    codeBuilderTitle = serializer::CommonTypes::getString(in);
    canResizeCodeBuilder = serializer::CommonTypes::getBool(in);
    disableLegacyTitleBar = serializer::CommonTypes::getBool(in);
    postProcessFilter = serializer::CommonTypes::getString(in);
    screenshotBorderResourcePath = serializer::CommonTypes::getString(in);
    agentCapabilities = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::EducationSettingsAgentCapabilities::read(reader); });
    codeBuilderOverrideUri = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });
    hasQuiz = serializer::CommonTypes::getBool(in);
    linkSettings = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::EducationSettingsExternalLinkSettings::read(reader); });

}

void EducationSettingsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, codeBuilderDefaultUri);
    serializer::CommonTypes::putString(out, codeBuilderTitle);
    serializer::CommonTypes::putBool(out, canResizeCodeBuilder);
    serializer::CommonTypes::putBool(out, disableLegacyTitleBar);
    serializer::CommonTypes::putString(out, postProcessFilter);
    serializer::CommonTypes::putString(out, screenshotBorderResourcePath);
    serializer::CommonTypes::writeOptional(out, agentCapabilities, [](encoding::ByteBufferWriter &out, const types::EducationSettingsAgentCapabilities &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(out, codeBuilderOverrideUri, [](encoding::ByteBufferWriter &writer, const std::string &value) { serializer::CommonTypes::putString(writer, value); });
    serializer::CommonTypes::putBool(out, hasQuiz);
    serializer::CommonTypes::writeOptional(out, linkSettings, [](encoding::ByteBufferWriter &out, const types::EducationSettingsExternalLinkSettings &v) { v.write(out); });

}

bool EducationSettingsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleEducationSettings(*this);
}

}  // namespace bedrock_protocol
