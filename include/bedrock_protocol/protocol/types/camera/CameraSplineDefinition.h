/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSplineDefinition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/camera/CameraSplineInstruction.h"

namespace bedrock_protocol::types::camera {

class CameraSplineDefinition final {
public:
    CameraSplineDefinition(std::string name, CameraSplineInstruction instruction)
        : name(std::move(name)), instruction(std::move(instruction))
    {
    }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const CameraSplineInstruction &getInstruction() const { return instruction; }

    /**
     * @throws DataDecodeException
     */
    static CameraSplineDefinition read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    CameraSplineInstruction instruction;
};

}  // namespace bedrock_protocol::types::camera
