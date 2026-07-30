/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPresetItemSettings.php
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

namespace bedrock_protocol::types::camera {

class CameraAimAssistPresetItemSettings final {
public:
    CameraAimAssistPresetItemSettings(std::string itemIdentifier, std::string categoryName)
        : itemIdentifier(std::move(itemIdentifier)), categoryName(std::move(categoryName))
    {
    }

    [[nodiscard]] const std::string &getItemIdentifier() const { return itemIdentifier; }

    [[nodiscard]] const std::string &getCategoryName() const { return categoryName; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistPresetItemSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string itemIdentifier;
    std::string categoryName;
};

}  // namespace bedrock_protocol::types::camera
