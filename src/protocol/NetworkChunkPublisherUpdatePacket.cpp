/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NetworkChunkPublisherUpdatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/NetworkChunkPublisherUpdatePacket.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

NetworkChunkPublisherUpdatePacket NetworkChunkPublisherUpdatePacket::create(types::BlockPosition blockPosition, std::uint32_t radius, std::vector<types::ChunkPosition> savedChunks)
{
    NetworkChunkPublisherUpdatePacket result;
    result.blockPosition = std::move(blockPosition);
    result.radius = radius;
    result.savedChunks = std::move(savedChunks);
    return result;
}

void NetworkChunkPublisherUpdatePacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    radius = encoding::VarInt::readUnsignedInt(in);

    const auto count = encoding::LE::readUnsignedInt(in);
    if (count > MAX_SAVED_CHUNKS) {
        throw PacketDecodeException("Expected at most " + std::to_string(MAX_SAVED_CHUNKS) + " saved chunks, got " + std::to_string(count));
    }
    savedChunks.clear();
    for (std::uint32_t i = 0; i < count; i++) {
        savedChunks.push_back(types::ChunkPosition::read(in));
    }

}

void NetworkChunkPublisherUpdatePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    encoding::VarInt::writeUnsignedInt(out, radius);

    encoding::LE::writeUnsignedInt(out, static_cast<std::uint32_t>(savedChunks.size()));
    for (const auto &chunk : savedChunks) {
        chunk.write(out);
    }

}

bool NetworkChunkPublisherUpdatePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleNetworkChunkPublisherUpdate(*this);
}

}  // namespace bedrock_protocol
