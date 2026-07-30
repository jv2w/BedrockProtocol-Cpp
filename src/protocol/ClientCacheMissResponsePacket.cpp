/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientCacheMissResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientCacheMissResponsePacket.h"

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

ClientCacheMissResponsePacket ClientCacheMissResponsePacket::create(std::vector<types::ChunkCacheBlob> blobs)
{
    ClientCacheMissResponsePacket result;
    result.blobs = std::move(blobs);
    return result;
}

void ClientCacheMissResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto hash = encoding::LE::readUnsignedLong(in);
        const auto payload = serializer::CommonTypes::getString(in);
        blobs.push_back(types::ChunkCacheBlob(hash, payload));
    }

}

void ClientCacheMissResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blobs.size()));
    for (const auto &blob : blobs) {
        encoding::LE::writeUnsignedLong(out, blob.getHash());
        serializer::CommonTypes::putString(out, blob.getPayload());
    }

}

bool ClientCacheMissResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientCacheMissResponse(*this);
}

}  // namespace bedrock_protocol
