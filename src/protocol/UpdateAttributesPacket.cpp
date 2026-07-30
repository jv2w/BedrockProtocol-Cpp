/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/UpdateAttributesPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateAttributesPacket.h"

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

UpdateAttributesPacket UpdateAttributesPacket::create(std::uint64_t actorRuntimeId, std::vector<types::entity::UpdateAttribute> entries, std::uint64_t tick)
{
    UpdateAttributesPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.entries = std::move(entries);
    result.tick = tick;
    return result;
}

void UpdateAttributesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        entries.push_back(types::entity::UpdateAttribute::read(in));
    }
    tick = encoding::VarInt::readUnsignedLong(in);

}

void UpdateAttributesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        entry.write(out);
    }
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool UpdateAttributesPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateAttributes(*this);
}

}  // namespace bedrock_protocol
