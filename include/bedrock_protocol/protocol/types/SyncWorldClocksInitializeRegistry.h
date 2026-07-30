/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SyncWorldClocksInitializeRegistry.php
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
#include "bedrock_protocol/protocol/types/SyncWorldClockData.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksPayload.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksType.h"

namespace bedrock_protocol::types {

/**
 * @see SyncWorldClocksPacket
 */
class SyncWorldClocksInitializeRegistry final : public SyncWorldClocksPayload {
public:
    static constexpr std::int32_t ID = SyncWorldClocksType::INITIALIZE_REGISTRY;

    explicit SyncWorldClocksInitializeRegistry(std::vector<SyncWorldClockData> clockData)
        : clockData(std::move(clockData))
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::vector<SyncWorldClockData> &getClockData() const { return clockData; }

    static SyncWorldClocksInitializeRegistry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<SyncWorldClocksPayload> clone() const override
    {
        return std::make_unique<SyncWorldClocksInitializeRegistry>(*this);
    }

private:
    std::vector<SyncWorldClockData> clockData;
};

}  // namespace bedrock_protocol::types
