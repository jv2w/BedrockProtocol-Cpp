/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SubChunkRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SubChunkRequestPacket.h"

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

SubChunkRequestPacket SubChunkRequestPacket::create(std::int32_t dimension, types::SubChunkPosition basePosition, std::vector<types::SubChunkPositionOffset> entries)
{
    SubChunkRequestPacket result;
    result.dimension = dimension;
    result.basePosition = std::move(basePosition);
    result.entries = std::move(entries);
    return result;
}

void SubChunkRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    dimension = encoding::VarInt::readSignedInt(in);

    entries.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        entries.push_back(types::SubChunkPositionOffset::read(in));
    }

    basePosition = types::SubChunkPosition::readFixedInts(in);

}

void SubChunkRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, dimension);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        entry.write(out);
    }

    basePosition.writeFixedInts(out);

}

bool SubChunkRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSubChunkRequest(*this);
}

}  // namespace bedrock_protocol
