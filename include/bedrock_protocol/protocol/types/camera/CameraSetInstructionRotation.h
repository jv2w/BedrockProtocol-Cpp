/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionRotation.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::camera {

class CameraSetInstructionRotation final {
public:
    CameraSetInstructionRotation(float pitch, float yaw) : pitch(pitch), yaw(yaw) {}

    [[nodiscard]] float getPitch() const { return pitch; }

    [[nodiscard]] float getYaw() const { return yaw; }

    /**
     * @throws DataDecodeException
     */
    static CameraSetInstructionRotation read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float pitch;
    float yaw;
};

}  // namespace bedrock_protocol::types::camera
