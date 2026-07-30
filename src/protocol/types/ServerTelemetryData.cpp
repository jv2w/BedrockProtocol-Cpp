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

#include "bedrock_protocol/protocol/types/ServerTelemetryData.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

ServerTelemetryData ServerTelemetryData::read(encoding::ByteBufferReader &in)
{
    auto serverId = CommonTypes::getString(in);
    auto scenarioId = CommonTypes::getString(in);
    auto worldId = CommonTypes::getString(in);
    auto ownerId = CommonTypes::getString(in);

    return ServerTelemetryData(
        std::move(serverId),
        std::move(scenarioId),
        std::move(worldId),
        std::move(ownerId)
    );
}

void ServerTelemetryData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, serverId);
    CommonTypes::putString(out, scenarioId);
    CommonTypes::putString(out, worldId);
    CommonTypes::putString(out, ownerId);
}

}  // namespace bedrock_protocol::types
