/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPreset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPreset.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::VarInt;
using serializer::CommonTypes;

CameraAimAssistPreset CameraAimAssistPreset::read(encoding::ByteBufferReader &in)
{
    auto identifier = CommonTypes::getString(in);
    auto exclusionList = CameraAimAssistPresetExclusionDefinition::read(in);

    std::vector<std::string> liquidTargetingList;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        liquidTargetingList.push_back(CommonTypes::getString(in));
    }

    std::vector<CameraAimAssistPresetItemSettings> itemSettings;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        itemSettings.push_back(CameraAimAssistPresetItemSettings::read(in));
    }

    auto defaultItemSettings =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getString(in); });
    auto defaultHandSettings =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getString(in); });

    return CameraAimAssistPreset(std::move(identifier), std::move(exclusionList), std::move(liquidTargetingList),
                                 std::move(itemSettings), std::move(defaultItemSettings),
                                 std::move(defaultHandSettings));
}

void CameraAimAssistPreset::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, identifier);
    exclusionSettings.write(out);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(liquidTargetingList.size()));
    for (const auto &liquidTargeting : liquidTargetingList) {
        CommonTypes::putString(out, liquidTargeting);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(itemSettings.size()));
    for (const auto &itemSetting : itemSettings) {
        itemSetting.write(out);
    }

    CommonTypes::writeOptional(out, defaultItemSettings, [](encoding::ByteBufferWriter &out, const std::string &v) {
        CommonTypes::putString(out, v);
    });
    CommonTypes::writeOptional(out, defaultHandSettings, [](encoding::ByteBufferWriter &out, const std::string &v) {
        CommonTypes::putString(out, v);
    });
}

}  // namespace bedrock_protocol::types::camera
