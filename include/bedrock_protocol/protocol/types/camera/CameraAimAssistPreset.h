/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPreset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPresetExclusionDefinition.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPresetItemSettings.h"

namespace bedrock_protocol::types::camera {

class CameraAimAssistPreset final {
public:
    CameraAimAssistPreset(std::string identifier, CameraAimAssistPresetExclusionDefinition exclusionSettings,
                          std::vector<std::string> liquidTargetingList,
                          std::vector<CameraAimAssistPresetItemSettings> itemSettings,
                          std::optional<std::string> defaultItemSettings,
                          std::optional<std::string> defaultHandSettings)
        : identifier(std::move(identifier)), exclusionSettings(std::move(exclusionSettings)),
          liquidTargetingList(std::move(liquidTargetingList)), itemSettings(std::move(itemSettings)),
          defaultItemSettings(std::move(defaultItemSettings)), defaultHandSettings(std::move(defaultHandSettings))
    {
    }

    [[nodiscard]] const std::string &getIdentifier() const { return identifier; }

    [[nodiscard]] const CameraAimAssistPresetExclusionDefinition &getExclusionSettings() const
    {
        return exclusionSettings;
    }

    [[nodiscard]] const std::vector<std::string> &getLiquidTargetingList() const { return liquidTargetingList; }

    [[nodiscard]] const std::vector<CameraAimAssistPresetItemSettings> &getItemSettings() const { return itemSettings; }

    [[nodiscard]] const std::optional<std::string> &getDefaultItemSettings() const { return defaultItemSettings; }

    [[nodiscard]] const std::optional<std::string> &getDefaultHandSettings() const { return defaultHandSettings; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistPreset read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string identifier;
    CameraAimAssistPresetExclusionDefinition exclusionSettings;
    std::vector<std::string> liquidTargetingList;
    std::vector<CameraAimAssistPresetItemSettings> itemSettings;
    std::optional<std::string> defaultItemSettings;
    std::optional<std::string> defaultHandSettings;
};

}  // namespace bedrock_protocol::types::camera
