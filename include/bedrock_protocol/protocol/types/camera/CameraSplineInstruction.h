/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraSplineInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/camera/CameraProgressOption.h"
#include "bedrock_protocol/protocol/types/camera/CameraRotationOption.h"

namespace bedrock_protocol::types::camera {

class CameraSplineInstruction final {
public:
    /**
     * @see CameraSetInstructionEaseType
     */
    CameraSplineInstruction(float totalTime, std::uint8_t easeType, std::vector<math::Vector3> curve,
                            std::vector<CameraProgressOption> progressKeyFrames,
                            std::vector<CameraRotationOption> rotationOptions, std::string splineIdentifier,
                            bool loadFromJson)
        : totalTime(totalTime), easeType(easeType), curve(std::move(curve)),
          progressKeyFrames(std::move(progressKeyFrames)), rotationOptions(std::move(rotationOptions)),
          splineIdentifier(std::move(splineIdentifier)), loadFromJson(loadFromJson)
    {
    }

    [[nodiscard]] float getTotalTime() const { return totalTime; }

    /**
     * @see CameraSetInstructionEaseType
     */
    [[nodiscard]] std::uint8_t getEaseType() const { return easeType; }

    [[nodiscard]] const std::vector<math::Vector3> &getCurve() const { return curve; }

    [[nodiscard]] const std::vector<CameraProgressOption> &getProgressKeyFrames() const { return progressKeyFrames; }

    [[nodiscard]] const std::vector<CameraRotationOption> &getRotationOptions() const { return rotationOptions; }

    [[nodiscard]] const std::string &getSplineIdentifier() const { return splineIdentifier; }

    [[nodiscard]] bool isLoadFromJson() const { return loadFromJson; }

    /**
     * @throws DataDecodeException
     */
    static CameraSplineInstruction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float totalTime;
    std::uint8_t easeType;
    std::vector<math::Vector3> curve;
    std::vector<CameraProgressOption> progressKeyFrames;
    std::vector<CameraRotationOption> rotationOptions;
    std::string splineIdentifier;
    bool loadFromJson;
};

}  // namespace bedrock_protocol::types::camera
