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

LevelChunkPacket LevelChunkPacket::create(types::ChunkPosition chunkPosition, std::int32_t dimensionId, std::uint32_t subChunkCount, std::optional<std::int32_t> subChunkLimit, bool cacheEnabled, std::vector<std::uint64_t> usedBlobHashes, std::string extraPayload)
{
    LevelChunkPacket result;
    result.chunkPosition = std::move(chunkPosition);
    result.dimensionId = dimensionId;
    result.subChunkCount = subChunkCount;
    result.subChunkLimit = subChunkLimit;
    result.cacheEnabled = cacheEnabled;
    result.usedBlobHashes = std::move(usedBlobHashes);
    result.extraPayload = std::move(extraPayload);
    return result;
}

void LevelChunkPacket::decodePayload(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/packet/level_chunk.go:44-52.
    chunkPosition = types::ChunkPosition::read(in);
    dimensionId = encoding::VarInt::readSignedInt(in);

    subChunkCount = encoding::VarInt::readUnsignedInt(in);
    if (subChunkCount > MAX_SUB_CHUNK_COUNT) {
        throw PacketDecodeException("Expected at most " + std::to_string(MAX_SUB_CHUNK_COUNT) + " sub-chunks, got " + std::to_string(subChunkCount));
    }

    subChunkLimit = serializer::CommonTypes::getBool(in) ? std::optional(encoding::VarInt::readSignedInt(in)) : std::nullopt;

    cacheEnabled = serializer::CommonTypes::getBool(in);

    usedBlobHashes.clear();
    const auto blobHashCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < blobHashCount; ++i) {
        usedBlobHashes.push_back(encoding::LE::readUnsignedLong(in));
    }

    extraPayload = serializer::CommonTypes::getString(in);

}

void LevelChunkPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    chunkPosition.write(out);
    encoding::VarInt::writeSignedInt(out, dimensionId);

    encoding::VarInt::writeUnsignedInt(out, subChunkCount);

    serializer::CommonTypes::putBool(out, subChunkLimit.has_value());
    if (subChunkLimit.has_value()) {
        encoding::VarInt::writeSignedInt(out, *subChunkLimit);
    }

    serializer::CommonTypes::putBool(out, cacheEnabled);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(usedBlobHashes.size()));
    for (const auto &hash : usedBlobHashes) {
        encoding::LE::writeUnsignedLong(out, hash);
    }

    serializer::CommonTypes::putString(out, extraPayload);

}

bool LevelChunkPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLevelChunk(*this);
}

}  // namespace bedrock_protocol
