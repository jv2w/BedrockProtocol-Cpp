/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClockMarkerData.php
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
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

/**
 * @see SyncWorldClockData&SyncWorldClocksAddTimeMarker
 */
class SyncWorldClockMarkerData final {
public:
    SyncWorldClockMarkerData(std::uint64_t id, std::string name, std::int32_t time, std::optional<std::int32_t> period)
        : id(id), name(std::move(name)), time(time), period(period)
    {
    }

    [[nodiscard]] std::uint64_t getId() const { return id; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] std::int32_t getTime() const { return time; }

    [[nodiscard]] const std::optional<std::int32_t> &getPeriod() const { return period; }

    static SyncWorldClockMarkerData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint64_t id;
    std::string name;
    std::int32_t time;
    std::optional<std::int32_t> period;
};

}  // namespace bedrock_protocol::types
