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

#include "bedrock_protocol/protocol/types/GatheringJoinInfo.h"

#include <string>
#include <utility>

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

GatheringJoinInfo GatheringJoinInfo::read(encoding::ByteBufferReader &in)
{
    const auto experienceId = CommonTypes::getUUID(in);
    auto experienceName = CommonTypes::getString(in);
    // gophertunnel v1.58.0 minecraft/protocol/server_join_information.go:29-34 -- these five gained a
    // presence byte each in 1.26.40.
    auto experienceWorldId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) { return CommonTypes::getUUID(r); });
    auto experienceWorldName =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) { return CommonTypes::getString(r); });
    auto creatorId = CommonTypes::getString(in);
    auto targetId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) { return CommonTypes::getUUID(r); });
    auto scenarioId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) { return CommonTypes::getString(r); });
    auto serverId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) { return CommonTypes::getString(r); });

    return GatheringJoinInfo(
        experienceId,
        std::move(experienceName),
        std::move(experienceWorldId),
        std::move(experienceWorldName),
        std::move(creatorId),
        std::move(targetId),
        std::move(scenarioId),
        std::move(serverId)
    );
}

void GatheringJoinInfo::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putUUID(out, experienceId);
    CommonTypes::putString(out, experienceName);
    CommonTypes::writeOptional(out, experienceWorldId,
                               [](encoding::ByteBufferWriter &w, const uuid::Uuid &v) { CommonTypes::putUUID(w, v); });
    CommonTypes::writeOptional(
        out, experienceWorldName,
        [](encoding::ByteBufferWriter &w, const std::string &v) { CommonTypes::putString(w, v); });
    CommonTypes::putString(out, creatorId);
    CommonTypes::writeOptional(out, targetId,
                               [](encoding::ByteBufferWriter &w, const uuid::Uuid &v) { CommonTypes::putUUID(w, v); });
    CommonTypes::writeOptional(
        out, scenarioId, [](encoding::ByteBufferWriter &w, const std::string &v) { CommonTypes::putString(w, v); });
    CommonTypes::writeOptional(
        out, serverId, [](encoding::ByteBufferWriter &w, const std::string &v) { CommonTypes::putString(w, v); });
}

}  // namespace bedrock_protocol::types
