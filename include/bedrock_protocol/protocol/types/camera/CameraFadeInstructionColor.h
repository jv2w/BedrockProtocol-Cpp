/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstructionColor.php
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

class CameraFadeInstructionColor final {
public:
    CameraFadeInstructionColor(float red, float green, float blue) : red(red), green(green), blue(blue) {}

    [[nodiscard]] float getRed() const { return red; }

    [[nodiscard]] float getGreen() const { return green; }

    [[nodiscard]] float getBlue() const { return blue; }

    /**
     * @throws DataDecodeException
     */
    static CameraFadeInstructionColor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float red;
    float green;
    float blue;
};

}  // namespace bedrock_protocol::types::camera
