/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/FeatureRegistryPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/FeatureRegistryPacket.h"

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

FeatureRegistryPacket FeatureRegistryPacket::create(std::vector<types::FeatureRegistryPacketEntry> entries)
{
    FeatureRegistryPacket result;
    result.entries = std::move(entries);
    return result;
}

void FeatureRegistryPacket::decodePayload(encoding::ByteBufferReader &in)
{
    entries.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; i++) {
        entries.push_back(types::FeatureRegistryPacketEntry::read(in));
    }

}

void FeatureRegistryPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        entry.write(out);
    }

}

bool FeatureRegistryPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleFeatureRegistry(*this);
}

}  // namespace bedrock_protocol
