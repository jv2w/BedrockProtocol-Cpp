/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CreativeContentPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CreativeContentPacket.h"

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

CreativeContentPacket CreativeContentPacket::create(std::vector<types::inventory::CreativeGroupEntry> groups, std::vector<types::inventory::CreativeItemEntry> items)
{
    CreativeContentPacket result;
    result.groups = std::move(groups);
    result.items = std::move(items);
    return result;
}

void CreativeContentPacket::decodePayload(encoding::ByteBufferReader &in)
{
    groups.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        groups.push_back(types::inventory::CreativeGroupEntry::read(in));
    }

    items.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        items.push_back(types::inventory::CreativeItemEntry::read(in));
    }

}

void CreativeContentPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(groups.size()));
    for (const auto &entry : groups) {
        entry.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(items.size()));
    for (const auto &entry : items) {
        entry.write(out);
    }

}

bool CreativeContentPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCreativeContent(*this);
}

}  // namespace bedrock_protocol
