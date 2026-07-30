/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionEase.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::camera {

class CameraSetInstructionEase final {
public:
    /**
     * @see CameraSetInstructionEaseType
     */
    CameraSetInstructionEase(std::uint8_t type, float duration) : type(type), duration(duration) {}

    /**
     * @see CameraSetInstructionEaseType
     */
    [[nodiscard]] std::uint8_t getType() const { return type; }

    [[nodiscard]] float getDuration() const { return duration; }

    /**
     * @throws DataDecodeException
     */
    static CameraSetInstructionEase read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t type;
    float duration;
};

}  // namespace bedrock_protocol::types::camera
