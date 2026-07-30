/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClockData.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/SyncWorldClockMarkerData.h"

namespace bedrock_protocol::types {

/**
 * @see SyncWorldClocksInitializeRegistry
 */
class SyncWorldClockData final {
public:
    SyncWorldClockData(std::uint64_t id, std::string name, std::int32_t time, bool paused, std::vector<SyncWorldClockMarkerData> markers)
        : id(id), name(std::move(name)), time(time), paused(paused), markers(std::move(markers))
    {
    }

    [[nodiscard]] std::uint64_t getId() const { return id; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] std::int32_t getTime() const { return time; }

    [[nodiscard]] bool isPaused() const { return paused; }

    [[nodiscard]] const std::vector<SyncWorldClockMarkerData> &getMarkers() const { return markers; }

    static SyncWorldClockData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint64_t id;
    std::string name;
    std::int32_t time;
    bool paused;
    std::vector<SyncWorldClockMarkerData> markers;
};

}  // namespace bedrock_protocol::types
