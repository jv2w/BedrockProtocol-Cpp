/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LevelChunkPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LevelChunkPacket.h"

#include <limits>
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

LevelChunkPacket LevelChunkPacket::create(types::ChunkPosition chunkPosition, std::int32_t dimensionId, std::int64_t subChunkCount, bool clientSubChunkRequestsEnabled, std::optional<std::vector<std::uint64_t>> usedBlobHashes, std::string extraPayload)
{
    LevelChunkPacket result;
    result.chunkPosition = std::move(chunkPosition);
    result.dimensionId = dimensionId;
    result.subChunkCount = subChunkCount;
    result.clientSubChunkRequestsEnabled = clientSubChunkRequestsEnabled;
    result.usedBlobHashes = std::move(usedBlobHashes);
    result.extraPayload = std::move(extraPayload);
    return result;
}

void LevelChunkPacket::decodePayload(encoding::ByteBufferReader &in)
{
    chunkPosition = types::ChunkPosition::read(in);
    dimensionId = encoding::VarInt::readSignedInt(in);

    const auto subChunkCountButNotReally = encoding::VarInt::readUnsignedInt(in);
    if (subChunkCountButNotReally == CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT) {
        clientSubChunkRequestsEnabled = true;
        subChunkCount = std::numeric_limits<std::int64_t>::max();
    }
    else if (subChunkCountButNotReally == CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT) {
        clientSubChunkRequestsEnabled = true;
        subChunkCount = encoding::LE::readUnsignedShort(in);
    }
    else {
        clientSubChunkRequestsEnabled = false;
        subChunkCount = subChunkCountButNotReally;
    }

    const auto cacheEnabled = serializer::CommonTypes::getBool(in);
    if (cacheEnabled) {
        usedBlobHashes.emplace();
        const auto count = encoding::VarInt::readUnsignedInt(in);
        if (count > MAX_BLOB_HASHES) {
            throw PacketDecodeException("Expected at most " + std::to_string(MAX_BLOB_HASHES) + " blob hashes, got " + std::to_string(count));
        }
        for (std::uint32_t i = 0; i < count; ++i) {
            usedBlobHashes->push_back(encoding::LE::readUnsignedLong(in));
        }
    }
    extraPayload = serializer::CommonTypes::getString(in);

}

void LevelChunkPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    chunkPosition.write(out);
    encoding::VarInt::writeSignedInt(out, dimensionId);

    if (clientSubChunkRequestsEnabled) {
        if (subChunkCount == std::numeric_limits<std::int64_t>::max()) {
            encoding::VarInt::writeUnsignedInt(out, CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT);
        }
        else {
            encoding::VarInt::writeUnsignedInt(out, CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT);
            encoding::LE::writeUnsignedShort(out, static_cast<std::uint16_t>(subChunkCount));
        }
    }
    else {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(subChunkCount));
    }

    serializer::CommonTypes::putBool(out, usedBlobHashes.has_value());
    if (usedBlobHashes.has_value()) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(usedBlobHashes->size()));
        for (const auto &hash : *usedBlobHashes) {
            encoding::LE::writeUnsignedLong(out, hash);
        }
    }
    serializer::CommonTypes::putString(out, extraPayload);

}

bool LevelChunkPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLevelChunk(*this);
}

}  // namespace bedrock_protocol
