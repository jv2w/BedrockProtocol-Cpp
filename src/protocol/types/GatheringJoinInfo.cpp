/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/GatheringJoinInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/GatheringJoinInfo.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

GatheringJoinInfo GatheringJoinInfo::read(encoding::ByteBufferReader &in)
{
    const auto experienceId = CommonTypes::getUUID(in);
    auto experienceName = CommonTypes::getString(in);
    const auto experienceWorldId = CommonTypes::getUUID(in);
    auto experienceWorldName = CommonTypes::getString(in);
    auto creatorId = CommonTypes::getString(in);
    const auto targetId = CommonTypes::getUUID(in);
    auto scenarioId = CommonTypes::getString(in);
    auto serverId = CommonTypes::getString(in);

    return GatheringJoinInfo(
        experienceId,
        std::move(experienceName),
        experienceWorldId,
        std::move(experienceWorldName),
        std::move(creatorId),
        targetId,
        std::move(scenarioId),
        std::move(serverId)
    );
}

void GatheringJoinInfo::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putUUID(out, experienceId);
    CommonTypes::putString(out, experienceName);
    CommonTypes::putUUID(out, experienceWorldId);
    CommonTypes::putString(out, experienceWorldName);
    CommonTypes::putString(out, creatorId);
    CommonTypes::putUUID(out, targetId);
    CommonTypes::putString(out, scenarioId);
    CommonTypes::putString(out, serverId);
}

}  // namespace bedrock_protocol::types
