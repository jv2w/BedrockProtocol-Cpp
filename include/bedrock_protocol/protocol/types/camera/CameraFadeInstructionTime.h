/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstructionTime.php
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

class CameraFadeInstructionTime final {
public:
    CameraFadeInstructionTime(float fadeInTime, float stayTime, float fadeOutTime)
        : fadeInTime(fadeInTime), stayTime(stayTime), fadeOutTime(fadeOutTime)
    {
    }

    [[nodiscard]] float getFadeInTime() const { return fadeInTime; }

    [[nodiscard]] float getStayTime() const { return stayTime; }

    [[nodiscard]] float getFadeOutTime() const { return fadeOutTime; }

    /**
     * @throws DataDecodeException
     */
    static CameraFadeInstructionTime read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float fadeInTime;
    float stayTime;
    float fadeOutTime;
};

}  // namespace bedrock_protocol::types::camera
