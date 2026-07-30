/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/LocatorBarWaypointPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/LocatorBarWaypoint.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types {

/**
 * @see LocatorBarPacket
 */
class LocatorBarWaypointPayload final {
public:
    LocatorBarWaypointPayload(uuid::Uuid group, LocatorBarWaypoint waypoint, std::uint8_t action)
        : group(group), waypoint(std::move(waypoint)), action(action)
    {
    }

    [[nodiscard]] const uuid::Uuid &getGroup() const { return group; }

    [[nodiscard]] const LocatorBarWaypoint &getWaypoint() const { return waypoint; }

    [[nodiscard]] std::uint8_t getAction() const { return action; }

    /**
     * @throws DataDecodeException
     */
    static LocatorBarWaypointPayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    uuid::Uuid group;
    LocatorBarWaypoint waypoint;
    std::uint8_t action;
};

}  // namespace bedrock_protocol::types
