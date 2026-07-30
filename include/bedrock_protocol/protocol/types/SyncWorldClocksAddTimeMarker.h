/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClocksAddTimeMarker.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/SyncWorldClockMarkerData.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksPayload.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksType.h"

namespace bedrock_protocol::types {

/**
 * @see SyncWorldClocksPacket
 */
class SyncWorldClocksAddTimeMarker final : public SyncWorldClocksPayload {
public:
    static constexpr std::int32_t ID = SyncWorldClocksType::ADD_TIME_MARKER;

    SyncWorldClocksAddTimeMarker(std::uint64_t clockId, std::vector<SyncWorldClockMarkerData> markers)
        : clockId(clockId), markers(std::move(markers))
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint64_t getClockId() const { return clockId; }

    [[nodiscard]] const std::vector<SyncWorldClockMarkerData> &getMarkers() const { return markers; }

    static SyncWorldClocksAddTimeMarker read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<SyncWorldClocksPayload> clone() const override
    {
        return std::make_unique<SyncWorldClocksAddTimeMarker>(*this);
    }

private:
    std::uint64_t clockId;
    std::vector<SyncWorldClockMarkerData> markers;
};

}  // namespace bedrock_protocol::types
