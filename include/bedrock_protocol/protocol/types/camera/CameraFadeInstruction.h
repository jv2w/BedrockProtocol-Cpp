/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/camera/CameraFadeInstructionColor.h"
#include "bedrock_protocol/protocol/types/camera/CameraFadeInstructionTime.h"

namespace bedrock_protocol::types::camera {

class CameraFadeInstruction final {
public:
    /**
     * @note The PHP original imports CameraFadeInstructionTime as Time and CameraFadeInstructionColor as Color;
     *       the C++ port spells both out in full.
     */
    CameraFadeInstruction(std::optional<CameraFadeInstructionTime> time,
                          std::optional<CameraFadeInstructionColor> color)
        : time(std::move(time)), color(std::move(color))
    {
    }

    [[nodiscard]] const std::optional<CameraFadeInstructionTime> &getTime() const { return time; }

    [[nodiscard]] const std::optional<CameraFadeInstructionColor> &getColor() const { return color; }

    /**
     * @throws DataDecodeException
     */
    static CameraFadeInstruction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<CameraFadeInstructionTime> time;
    std::optional<CameraFadeInstructionColor> color;
};

}  // namespace bedrock_protocol::types::camera
