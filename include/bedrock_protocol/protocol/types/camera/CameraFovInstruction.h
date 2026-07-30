/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraFovInstruction.php
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

class CameraFovInstruction final {
public:
    /**
     * @note The PHP original takes int|string for easeType; the C++ port splits that into two constructors.
     */
    CameraFovInstruction(float fieldOfView, float easeTime, std::uint8_t easeType, bool clear)
        : fieldOfView(fieldOfView), easeTime(easeTime), easeType(CameraSetInstructionEaseType::toName(easeType)),
          clear(clear)
    {
    }

    CameraFovInstruction(float fieldOfView, float easeTime, std::string easeType, bool clear)
        : fieldOfView(fieldOfView), easeTime(easeTime), easeType(std::move(easeType)), clear(clear)
    {
    }

    [[nodiscard]] float getFieldOfView() const { return fieldOfView; }

    [[nodiscard]] float getEaseTime() const { return easeTime; }

    /**
     * @see CameraSetInstructionEaseType
     */
    [[nodiscard]] const std::string &getEaseType() const { return easeType; }

    [[nodiscard]] bool getClear() const { return clear; }

    /**
     * @throws DataDecodeException
     */
    static CameraFovInstruction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float fieldOfView;
    float easeTime;
    /** @see CameraSetInstructionEaseType */
    std::string easeType;
    bool clear;
};

}  // namespace bedrock_protocol::types::camera
