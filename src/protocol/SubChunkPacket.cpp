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
#include <vector>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

SubChunkPacket SubChunkPacket::create(bool cacheEnabled, std::int32_t dimension, types::SubChunkPosition baseSubChunkPosition, std::vector<types::SubChunkPacketEntry> entries)
{
    SubChunkPacket result;
    result.cacheEnabled = cacheEnabled;
    result.dimension = dimension;
    result.baseSubChunkPosition = std::move(baseSubChunkPosition);
    result.entries = std::move(entries);
    return result;
}

void SubChunkPacket::decodePayload(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/packet/sub_chunk.go:25-30. The position is three fixed
    //LE int32s (minecraft/protocol/reader.go:141-145), and the entry list is varuint32-counted.
    cacheEnabled = serializer::CommonTypes::getBool(in);
    dimension = encoding::VarInt::readSignedInt(in);
    baseSubChunkPosition = types::SubChunkPosition::readFixedInts(in);

    entries.clear();
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; i++) {
        entries.push_back(types::SubChunkPacketEntry::read(in));
    }
}

void SubChunkPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, cacheEnabled);
    encoding::VarInt::writeSignedInt(out, dimension);
    baseSubChunkPosition.writeFixedInts(out);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        entry.write(out);
    }
}

bool SubChunkPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSubChunk(*this);
}

}  // namespace bedrock_protocol
