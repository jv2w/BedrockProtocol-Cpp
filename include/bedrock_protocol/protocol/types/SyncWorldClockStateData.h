/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SyncWorldClockStateData.php
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

/**
 * @see SyncWorldClocksSyncState
 */
class SyncWorldClockStateData final {
public:
    SyncWorldClockStateData(std::uint64_t clockId, std::int32_t time, bool paused)
        : clockId(clockId), time(time), paused(paused)
    {
    }

    [[nodiscard]] std::uint64_t getClockId() const { return clockId; }

    [[nodiscard]] std::int32_t getTime() const { return time; }

    [[nodiscard]] bool isPaused() const { return paused; }

    static SyncWorldClockStateData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint64_t clockId;
    std::int32_t time;
    bool paused;
};

}  // namespace bedrock_protocol::types
