/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistCategoryPriority.php
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

namespace bedrock_protocol::types::camera {

class CameraAimAssistCategoryPriority final {
public:
    CameraAimAssistCategoryPriority(std::string identifier, std::int32_t priority)
        : identifier(std::move(identifier)), priority(priority)
    {
    }

    [[nodiscard]] const std::string &getIdentifier() const { return identifier; }

    [[nodiscard]] std::int32_t getPriority() const { return priority; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistCategoryPriority read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string identifier;
    std::int32_t priority;
};

}  // namespace bedrock_protocol::types::camera
