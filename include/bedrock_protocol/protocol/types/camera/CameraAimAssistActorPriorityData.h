/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistActorPriorityData.php
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

class CameraAimAssistActorPriorityData final {
public:
    CameraAimAssistActorPriorityData(std::int32_t presetIndex, std::int32_t categoryIndex, std::int32_t actorIndex,
                                     std::int32_t priority)
        : presetIndex(presetIndex), categoryIndex(categoryIndex), actorIndex(actorIndex), priority(priority)
    {
    }

    [[nodiscard]] std::int32_t getPresetIndex() const { return presetIndex; }

    [[nodiscard]] std::int32_t getCategoryIndex() const { return categoryIndex; }

    [[nodiscard]] std::int32_t getActorIndex() const { return actorIndex; }

    [[nodiscard]] std::int32_t getPriority() const { return priority; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistActorPriorityData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t presetIndex;
    std::int32_t categoryIndex;
    std::int32_t actorIndex;
    std::int32_t priority;
};

}  // namespace bedrock_protocol::types::camera
