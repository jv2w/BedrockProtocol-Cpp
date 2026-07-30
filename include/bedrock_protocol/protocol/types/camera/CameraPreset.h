/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraPreset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/ControlScheme.h"
#include "bedrock_protocol/protocol/types/camera/CameraPresetAimAssist.h"

namespace bedrock_protocol::types::camera {

class CameraPreset final {
public:
    static constexpr std::uint8_t AUDIO_LISTENER_TYPE_CAMERA = 0;
    static constexpr std::uint8_t AUDIO_LISTENER_TYPE_PLAYER = 1;

    CameraPreset(std::string name, std::string parent, std::optional<float> xPosition, std::optional<float> yPosition,
                 std::optional<float> zPosition, std::optional<float> pitch, std::optional<float> yaw,
                 std::optional<float> rotationSpeed, std::optional<bool> snapToTarget,
                 std::optional<math::Vector2> horizontalRotationLimit,
                 std::optional<math::Vector2> verticalRotationLimit, std::optional<bool> continueTargeting,
                 std::optional<float> blockListeningRadius, std::optional<math::Vector2> viewOffset,
                 std::optional<math::Vector3> entityOffset, std::optional<float> radius,
                 std::optional<float> yawLimitMin, std::optional<float> yawLimitMax,
                 std::optional<std::uint8_t> audioListenerType, std::optional<bool> playerEffects,
                 std::optional<CameraPresetAimAssist> aimAssist, std::optional<ControlScheme> controlScheme)
        : name(std::move(name)), parent(std::move(parent)), xPosition(xPosition), yPosition(yPosition),
          zPosition(zPosition), pitch(pitch), yaw(yaw), rotationSpeed(rotationSpeed), snapToTarget(snapToTarget),
          horizontalRotationLimit(std::move(horizontalRotationLimit)),
          verticalRotationLimit(std::move(verticalRotationLimit)), continueTargeting(continueTargeting),
          blockListeningRadius(blockListeningRadius), viewOffset(std::move(viewOffset)),
          entityOffset(std::move(entityOffset)), radius(radius), yawLimitMin(yawLimitMin), yawLimitMax(yawLimitMax),
          audioListenerType(audioListenerType), playerEffects(playerEffects), aimAssist(std::move(aimAssist)),
          controlScheme(controlScheme)
    {
    }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const std::string &getParent() const { return parent; }

    [[nodiscard]] const std::optional<float> &getXPosition() const { return xPosition; }

    [[nodiscard]] const std::optional<float> &getYPosition() const { return yPosition; }

    [[nodiscard]] const std::optional<float> &getZPosition() const { return zPosition; }

    [[nodiscard]] const std::optional<float> &getPitch() const { return pitch; }

    [[nodiscard]] const std::optional<float> &getYaw() const { return yaw; }

    [[nodiscard]] const std::optional<float> &getRotationSpeed() const { return rotationSpeed; }

    [[nodiscard]] const std::optional<bool> &getSnapToTarget() const { return snapToTarget; }

    [[nodiscard]] const std::optional<math::Vector2> &getHorizontalRotationLimit() const
    {
        return horizontalRotationLimit;
    }

    [[nodiscard]] const std::optional<math::Vector2> &getVerticalRotationLimit() const { return verticalRotationLimit; }

    [[nodiscard]] const std::optional<bool> &getContinueTargeting() const { return continueTargeting; }

    [[nodiscard]] const std::optional<float> &getBlockListeningRadius() const { return blockListeningRadius; }

    [[nodiscard]] const std::optional<math::Vector2> &getViewOffset() const { return viewOffset; }

    [[nodiscard]] const std::optional<math::Vector3> &getEntityOffset() const { return entityOffset; }

    [[nodiscard]] const std::optional<float> &getRadius() const { return radius; }

    [[nodiscard]] const std::optional<float> &getYawLimitMin() const { return yawLimitMin; }

    [[nodiscard]] const std::optional<float> &getYawLimitMax() const { return yawLimitMax; }

    [[nodiscard]] const std::optional<std::uint8_t> &getAudioListenerType() const { return audioListenerType; }

    [[nodiscard]] const std::optional<bool> &getPlayerEffects() const { return playerEffects; }

    [[nodiscard]] const std::optional<CameraPresetAimAssist> &getAimAssist() const { return aimAssist; }

    [[nodiscard]] const std::optional<ControlScheme> &getControlScheme() const { return controlScheme; }

    /**
     * @throws DataDecodeException
     */
    static CameraPreset read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::string parent;
    std::optional<float> xPosition;
    std::optional<float> yPosition;
    std::optional<float> zPosition;
    std::optional<float> pitch;
    std::optional<float> yaw;
    std::optional<float> rotationSpeed;
    std::optional<bool> snapToTarget;
    std::optional<math::Vector2> horizontalRotationLimit;
    std::optional<math::Vector2> verticalRotationLimit;
    std::optional<bool> continueTargeting;
    std::optional<float> blockListeningRadius;
    std::optional<math::Vector2> viewOffset;
    std::optional<math::Vector3> entityOffset;
    std::optional<float> radius;
    std::optional<float> yawLimitMin;
    std::optional<float> yawLimitMax;
    std::optional<std::uint8_t> audioListenerType;
    std::optional<bool> playerEffects;
    std::optional<CameraPresetAimAssist> aimAssist;
    std::optional<ControlScheme> controlScheme;
};

}  // namespace bedrock_protocol::types::camera
