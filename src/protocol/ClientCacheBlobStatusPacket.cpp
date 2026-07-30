/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientCacheBlobStatusPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientCacheBlobStatusPacket.h"

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

ClientCacheBlobStatusPacket ClientCacheBlobStatusPacket::create(std::vector<std::uint64_t> hitHashes, std::vector<std::uint64_t> missHashes)
{
    ClientCacheBlobStatusPacket result;
    result.hitHashes = std::move(hitHashes);
    result.missHashes = std::move(missHashes);
    return result;
}

void ClientCacheBlobStatusPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto missCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < missCount; ++i) {
        missHashes.push_back(encoding::LE::readUnsignedLong(in));
    }
    const auto hitCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < hitCount; ++i) {
        hitHashes.push_back(encoding::LE::readUnsignedLong(in));
    }

}

void ClientCacheBlobStatusPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(missHashes.size()));
    for (const auto &hash : missHashes) {
        encoding::LE::writeUnsignedLong(out, hash);
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(hitHashes.size()));
    for (const auto &hash : hitHashes) {
        encoding::LE::writeUnsignedLong(out, hash);
    }

}

bool ClientCacheBlobStatusPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientCacheBlobStatus(*this);
}

}  // namespace bedrock_protocol
