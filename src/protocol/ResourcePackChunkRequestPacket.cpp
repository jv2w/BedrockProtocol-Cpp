/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePackChunkRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackChunkRequestPacket.h"

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

ResourcePackChunkRequestPacket ResourcePackChunkRequestPacket::create(std::string packId, std::uint32_t chunkIndex)
{
    ResourcePackChunkRequestPacket result;
    result.packId = std::move(packId);
    result.chunkIndex = chunkIndex;
    return result;
}

void ResourcePackChunkRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    packId = serializer::CommonTypes::getString(in);
    chunkIndex = encoding::LE::readUnsignedInt(in);

}

void ResourcePackChunkRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, packId);
    encoding::LE::writeUnsignedInt(out, chunkIndex);

}

bool ResourcePackChunkRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackChunkRequest(*this);
}

}  // namespace bedrock_protocol
