/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraTargetInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol::types::camera {

class CameraTargetInstruction final {
public:
    CameraTargetInstruction(std::optional<math::Vector3> targetCenterOffset, std::int64_t actorUniqueId)
        : targetCenterOffset(std::move(targetCenterOffset)), actorUniqueId(actorUniqueId)
    {
    }

    [[nodiscard]] const std::optional<math::Vector3> &getTargetCenterOffset() const { return targetCenterOffset; }

    [[nodiscard]] std::int64_t getActorUniqueId() const { return actorUniqueId; }

    /**
     * @throws DataDecodeException
     */
    static CameraTargetInstruction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<math::Vector3> targetCenterOffset;
    std::int64_t actorUniqueId;
};

}  // namespace bedrock_protocol::types::camera
