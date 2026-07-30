/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistCategory.php
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
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategoryPriorities.h"

namespace bedrock_protocol::types::camera {

class CameraAimAssistCategory final {
public:
    CameraAimAssistCategory(std::string name, CameraAimAssistCategoryPriorities priorities)
        : name(std::move(name)), priorities(std::move(priorities))
    {
    }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const CameraAimAssistCategoryPriorities &getPriorities() const { return priorities; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistCategory read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    CameraAimAssistCategoryPriorities priorities;
};

}  // namespace bedrock_protocol::types::camera
