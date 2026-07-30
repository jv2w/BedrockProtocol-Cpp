/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraProgressOption.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionEaseType.h"

namespace bedrock_protocol::types::camera {

class CameraProgressOption final {
public:
    /**
     * @note The PHP original takes int|string for easeType; the C++ port splits that into two constructors.
     */
    CameraProgressOption(float value, float time, std::uint8_t easeType)
        : value(value), time(time), easeType(CameraSetInstructionEaseType::toName(easeType))
    {
    }

    CameraProgressOption(float value, float time, std::string easeType)
        : value(value), time(time), easeType(std::move(easeType))
    {
    }

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] float getTime() const { return time; }

    /**
     * @see CameraSetInstructionEaseType
     */
    [[nodiscard]] const std::string &getEaseType() const { return easeType; }

    /**
     * @throws DataDecodeException
     */
    static CameraProgressOption read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float value;
    float time;
    /** @see CameraSetInstructionEaseType */
    std::string easeType;
};

}  // namespace bedrock_protocol::types::camera
