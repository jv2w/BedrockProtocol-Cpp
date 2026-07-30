/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateSubChunkBlocksPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateSubChunkBlocksPacket.h"

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

UpdateSubChunkBlocksPacket UpdateSubChunkBlocksPacket::create(types::BlockPosition baseBlockPosition, std::vector<types::UpdateSubChunkBlocksPacketEntry> layer0Updates, std::vector<types::UpdateSubChunkBlocksPacketEntry> layer1Updates)
{
    UpdateSubChunkBlocksPacket result;
    result.baseBlockPosition = std::move(baseBlockPosition);
    result.layer0Updates = std::move(layer0Updates);
    result.layer1Updates = std::move(layer1Updates);
    return result;
}

void UpdateSubChunkBlocksPacket::decodePayload(encoding::ByteBufferReader &in)
{
    baseBlockPosition = serializer::CommonTypes::getBlockPosition(in);
    layer0Updates.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        layer0Updates.push_back(types::UpdateSubChunkBlocksPacketEntry::read(in));
    }
    layer1Updates.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        layer1Updates.push_back(types::UpdateSubChunkBlocksPacketEntry::read(in));
    }

}

void UpdateSubChunkBlocksPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, baseBlockPosition);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(layer0Updates.size()));
    for (const auto &update : layer0Updates) {
        update.write(out);
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(layer1Updates.size()));
    for (const auto &update : layer1Updates) {
        update.write(out);
    }

}

bool UpdateSubChunkBlocksPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateSubChunkBlocks(*this);
}

}  // namespace bedrock_protocol
