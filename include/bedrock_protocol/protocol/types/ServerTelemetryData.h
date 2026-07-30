/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ServerTelemetryData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class ServerTelemetryData final {
public:
    ServerTelemetryData(std::string serverId, std::string scenarioId, std::string worldId, std::string ownerId)
        : serverId(std::move(serverId)), scenarioId(std::move(scenarioId)), worldId(std::move(worldId)), ownerId(std::move(ownerId))
    {
    }

    [[nodiscard]] const std::string &getServerId() const { return serverId; }

    [[nodiscard]] const std::string &getScenarioId() const { return scenarioId; }

    [[nodiscard]] const std::string &getWorldId() const { return worldId; }

    [[nodiscard]] const std::string &getOwnerId() const { return ownerId; }

    static ServerTelemetryData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string serverId;
    std::string scenarioId;
    std::string worldId;
    std::string ownerId;
};

}  // namespace bedrock_protocol::types
