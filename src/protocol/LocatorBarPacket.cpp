/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LocatorBarPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LocatorBarPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

LocatorBarPacket LocatorBarPacket::create(std::vector<types::LocatorBarWaypointPayload> waypoints)
{
    LocatorBarPacket result;
    result.waypoints = std::move(waypoints);
    return result;
}

void LocatorBarPacket::decodePayload(encoding::ByteBufferReader &in)
{
    waypoints.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        waypoints.push_back(types::LocatorBarWaypointPayload::read(in));
    }

}

void LocatorBarPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(waypoints.size()));
    for (const auto &waypoint : waypoints) {
        waypoint.write(out);
    }

}

bool LocatorBarPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLocatorBar(*this);
}

}  // namespace bedrock_protocol
