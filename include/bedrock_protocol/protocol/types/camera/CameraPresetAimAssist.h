/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraPresetAimAssist.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistTargetMode.h"

namespace bedrock_protocol::types::camera {

class CameraPresetAimAssist final {
public:
    CameraPresetAimAssist(std::optional<std::string> presetId, std::optional<CameraAimAssistTargetMode> targetMode,
                          std::optional<math::Vector2> viewAngle, std::optional<float> distance)
        : presetId(std::move(presetId)), targetMode(targetMode), viewAngle(std::move(viewAngle)), distance(distance)
    {
    }

    [[nodiscard]] const std::optional<std::string> &getPresetId() const { return presetId; }

    [[nodiscard]] const std::optional<CameraAimAssistTargetMode> &getTargetMode() const { return targetMode; }

    [[nodiscard]] const std::optional<math::Vector2> &getViewAngle() const { return viewAngle; }

    [[nodiscard]] const std::optional<float> &getDistance() const { return distance; }

    /**
     * @throws DataDecodeException
     */
    static CameraPresetAimAssist read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<std::string> presetId;
    std::optional<CameraAimAssistTargetMode> targetMode;
    std::optional<math::Vector2> viewAngle;
    std::optional<float> distance;
};

}  // namespace bedrock_protocol::types::camera
