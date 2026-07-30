/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraSetInstruction.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionEase.h"
#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionRotation.h"

namespace bedrock_protocol::types::camera {

class CameraSetInstruction final {
public:
    CameraSetInstruction(std::uint32_t preset, std::optional<CameraSetInstructionEase> ease,
                         std::optional<math::Vector3> cameraPosition,
                         std::optional<CameraSetInstructionRotation> rotation,
                         std::optional<math::Vector3> facingPosition, std::optional<math::Vector2> viewOffset,
                         std::optional<math::Vector3> entityOffset, std::optional<bool> default_,
                         bool ignoreStartingValuesComponent)
        : preset(preset), ease(std::move(ease)), cameraPosition(std::move(cameraPosition)),
          rotation(std::move(rotation)), facingPosition(std::move(facingPosition)), viewOffset(std::move(viewOffset)),
          entityOffset(std::move(entityOffset)), default_(default_),
          ignoreStartingValuesComponent(ignoreStartingValuesComponent)
    {
    }

    [[nodiscard]] std::uint32_t getPreset() const { return preset; }

    [[nodiscard]] const std::optional<CameraSetInstructionEase> &getEase() const { return ease; }

    [[nodiscard]] const std::optional<math::Vector3> &getCameraPosition() const { return cameraPosition; }

    [[nodiscard]] const std::optional<CameraSetInstructionRotation> &getRotation() const { return rotation; }

    [[nodiscard]] const std::optional<math::Vector3> &getFacingPosition() const { return facingPosition; }

    [[nodiscard]] const std::optional<math::Vector2> &getViewOffset() const { return viewOffset; }

    [[nodiscard]] const std::optional<math::Vector3> &getEntityOffset() const { return entityOffset; }

    [[nodiscard]] const std::optional<bool> &getDefault() const { return default_; }

    [[nodiscard]] bool isIgnoringStartingValuesComponent() const { return ignoreStartingValuesComponent; }

    /**
     * @throws DataDecodeException
     */
    static CameraSetInstruction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t preset;
    std::optional<CameraSetInstructionEase> ease;
    std::optional<math::Vector3> cameraPosition;
    std::optional<CameraSetInstructionRotation> rotation;
    std::optional<math::Vector3> facingPosition;
    std::optional<math::Vector2> viewOffset;
    std::optional<math::Vector3> entityOffset;
    std::optional<bool> default_;  //named $default in PHP; "default" is a reserved word in C++
    bool ignoreStartingValuesComponent;
};

}  // namespace bedrock_protocol::types::camera
