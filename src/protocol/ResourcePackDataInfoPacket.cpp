/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ResourcePackDataInfoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackDataInfoPacket.h"

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

ResourcePackDataInfoPacket ResourcePackDataInfoPacket::create(std::string packId, std::uint32_t maxChunkSize, std::uint32_t chunkCount, std::uint64_t compressedPackSize, std::string sha256, bool isPremium, std::uint8_t packType)
{
    ResourcePackDataInfoPacket result;
    result.packId = std::move(packId);
    result.maxChunkSize = maxChunkSize;
    result.chunkCount = chunkCount;
    result.compressedPackSize = compressedPackSize;
    result.sha256 = std::move(sha256);
    result.isPremium = isPremium;
    result.packType = packType;
    return result;
}

void ResourcePackDataInfoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    packId = serializer::CommonTypes::getString(in);
    maxChunkSize = encoding::LE::readUnsignedInt(in);
    chunkCount = encoding::LE::readUnsignedInt(in);
    compressedPackSize = encoding::LE::readUnsignedLong(in);
    sha256 = serializer::CommonTypes::getString(in);
    isPremium = serializer::CommonTypes::getBool(in);
    packType = encoding::Byte::readUnsigned(in);

}

void ResourcePackDataInfoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, packId);
    encoding::LE::writeUnsignedInt(out, maxChunkSize);
    encoding::LE::writeUnsignedInt(out, chunkCount);
    encoding::LE::writeUnsignedLong(out, compressedPackSize);
    serializer::CommonTypes::putString(out, sha256);
    serializer::CommonTypes::putBool(out, isPremium);
    encoding::Byte::writeUnsigned(out, packType);

}

bool ResourcePackDataInfoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackDataInfo(*this);
}

}  // namespace bedrock_protocol
