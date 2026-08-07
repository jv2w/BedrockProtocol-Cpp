/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/GatheringJoinInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types {

class GatheringJoinInfo final {
public:
    GatheringJoinInfo(const uuid::Uuid &experienceId, std::string experienceName,
                      std::optional<uuid::Uuid> experienceWorldId, std::optional<std::string> experienceWorldName,
                      std::string creatorId, std::optional<uuid::Uuid> targetId,
                      std::optional<std::string> scenarioId, std::optional<std::string> serverId)
        : experienceId(experienceId), experienceName(std::move(experienceName)),
          experienceWorldId(experienceWorldId), experienceWorldName(std::move(experienceWorldName)),
          creatorId(std::move(creatorId)), targetId(targetId), scenarioId(std::move(scenarioId)),
          serverId(std::move(serverId))
    {
    }

    [[nodiscard]] const uuid::Uuid &getExperienceId() const { return experienceId; }

    [[nodiscard]] const std::string &getExperienceName() const { return experienceName; }

    [[nodiscard]] const std::optional<uuid::Uuid> &getExperienceWorldId() const { return experienceWorldId; }

    [[nodiscard]] const std::optional<std::string> &getExperienceWorldName() const { return experienceWorldName; }

    [[nodiscard]] const std::string &getCreatorId() const { return creatorId; }

    [[nodiscard]] const std::optional<uuid::Uuid> &getTargetId() const { return targetId; }

    [[nodiscard]] const std::optional<std::string> &getScenarioId() const { return scenarioId; }

    [[nodiscard]] const std::optional<std::string> &getServerId() const { return serverId; }

    /** @throws DataDecodeException */
    static GatheringJoinInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    uuid::Uuid experienceId;
    std::string experienceName;
    std::optional<uuid::Uuid> experienceWorldId;
    std::optional<std::string> experienceWorldName;
    std::string creatorId;
    std::optional<uuid::Uuid> targetId;
    std::optional<std::string> scenarioId;
    std::optional<std::string> serverId;
};

}  // namespace bedrock_protocol::types
