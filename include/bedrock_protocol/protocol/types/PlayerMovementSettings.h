/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayerMovementSettings.php
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

namespace bedrock_protocol::types {

class PlayerMovementSettings final {
public:
    PlayerMovementSettings(std::int32_t rewindHistorySize, bool serverAuthoritativeBlockBreaking)
        : rewindHistorySize(rewindHistorySize), serverAuthoritativeBlockBreaking(serverAuthoritativeBlockBreaking)
    {
    }

    [[nodiscard]] std::int32_t getRewindHistorySize() const { return rewindHistorySize; }

    [[nodiscard]] bool isServerAuthoritativeBlockBreaking() const { return serverAuthoritativeBlockBreaking; }

    static PlayerMovementSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t rewindHistorySize;
    bool serverAuthoritativeBlockBreaking;
};

}  // namespace bedrock_protocol::types
