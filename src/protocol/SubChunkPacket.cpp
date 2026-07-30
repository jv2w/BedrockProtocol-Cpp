/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SubChunkPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SubChunkPacket.h"

#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

SubChunkPacket SubChunkPacket::create(std::int32_t dimension, types::SubChunkPosition baseSubChunkPosition, std::variant<types::SubChunkPacketEntryWithCacheList, types::SubChunkPacketEntryWithoutCacheList> entries)
{
    SubChunkPacket result;
    result.dimension = dimension;
    result.baseSubChunkPosition = std::move(baseSubChunkPosition);
    result.entries = std::move(entries);
    return result;
}

void SubChunkPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto cacheEnabled = serializer::CommonTypes::getBool(in);
    dimension = encoding::VarInt::readSignedInt(in);
    baseSubChunkPosition = types::SubChunkPosition::readVarInts(in);

    const auto count = encoding::LE::readUnsignedInt(in);
    if (cacheEnabled) {
        std::vector<types::SubChunkPacketEntryWithCache> newEntries;
        for (std::uint32_t i = 0; i < count; i++) {
            newEntries.push_back(types::SubChunkPacketEntryWithCache::read(in));
        }
        entries = types::SubChunkPacketEntryWithCacheList(std::move(newEntries));
    }
    else {
        std::vector<types::SubChunkPacketEntryWithoutCache> newEntries;
        for (std::uint32_t i = 0; i < count; i++) {
            newEntries.push_back(types::SubChunkPacketEntryWithoutCache::read(in));
        }
        entries = types::SubChunkPacketEntryWithoutCacheList(std::move(newEntries));
    }
}

void SubChunkPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, std::holds_alternative<types::SubChunkPacketEntryWithCacheList>(entries));
    encoding::VarInt::writeSignedInt(out, dimension);
    baseSubChunkPosition.writeVarInts(out);

    std::visit(
        [&out](const auto &list) {
            encoding::LE::writeUnsignedInt(out, static_cast<std::uint32_t>(list.getEntries().size()));

            for (const auto &entry : list.getEntries()) {
                entry.write(out);
            }
        },
        entries);
}

bool SubChunkPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSubChunk(*this);
}

}  // namespace bedrock_protocol
