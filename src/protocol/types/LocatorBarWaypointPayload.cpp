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

#include "bedrock_protocol/protocol/types/LocatorBarWaypointPayload.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using serializer::CommonTypes;

LocatorBarWaypointPayload LocatorBarWaypointPayload::read(encoding::ByteBufferReader &in)
{
    const auto group = CommonTypes::getUUID(in);
    auto waypoint = LocatorBarWaypoint::read(in);
    const auto action = Byte::readUnsigned(in);

    return LocatorBarWaypointPayload(group, std::move(waypoint), action);
}

void LocatorBarWaypointPayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putUUID(out, group);
    waypoint.write(out);
    Byte::writeUnsigned(out, action);
}

}  // namespace bedrock_protocol::types
