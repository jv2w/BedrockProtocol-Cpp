/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePackChunkDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackChunkDataPacket.h"

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

ResourcePackChunkDataPacket ResourcePackChunkDataPacket::create(std::string packId, std::uint32_t chunkIndex, std::uint64_t offset, std::string data)
{
    ResourcePackChunkDataPacket result;
    result.packId = std::move(packId);
    result.chunkIndex = chunkIndex;
    result.offset = offset;
    result.data = std::move(data);
    return result;
}

void ResourcePackChunkDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    packId = serializer::CommonTypes::getString(in);
    chunkIndex = encoding::LE::readUnsignedInt(in);
    offset = encoding::LE::readUnsignedLong(in);
    data = serializer::CommonTypes::getString(in);

}

void ResourcePackChunkDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, packId);
    encoding::LE::writeUnsignedInt(out, chunkIndex);
    encoding::LE::writeUnsignedLong(out, offset);
    serializer::CommonTypes::putString(out, data);

}

bool ResourcePackChunkDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackChunkData(*this);
}

}  // namespace bedrock_protocol
