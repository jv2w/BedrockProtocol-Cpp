/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EducationSettingsAgentCapabilities.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class EducationSettingsAgentCapabilities final {
public:
    explicit EducationSettingsAgentCapabilities(std::optional<bool> canModifyBlocks) : canModifyBlocks(canModifyBlocks)
    {
    }

    [[nodiscard]] std::optional<bool> getCanModifyBlocks() const { return canModifyBlocks; }

    /** @throws DataDecodeException */
    static EducationSettingsAgentCapabilities read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<bool> canModifyBlocks;
};

}  // namespace bedrock_protocol::types
