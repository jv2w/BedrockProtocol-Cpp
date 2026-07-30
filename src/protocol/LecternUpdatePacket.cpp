/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LecternUpdatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LecternUpdatePacket.h"

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

LecternUpdatePacket LecternUpdatePacket::create(std::uint8_t page, std::uint8_t totalPages, types::BlockPosition blockPosition)
{
    LecternUpdatePacket result;
    result.page = page;
    result.totalPages = totalPages;
    result.blockPosition = std::move(blockPosition);
    return result;
}

void LecternUpdatePacket::decodePayload(encoding::ByteBufferReader &in)
{
    page = encoding::Byte::readUnsigned(in);
    totalPages = encoding::Byte::readUnsigned(in);
    blockPosition = serializer::CommonTypes::getBlockPosition(in);

}

void LecternUpdatePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, page);
    encoding::Byte::writeUnsigned(out, totalPages);
    serializer::CommonTypes::putBlockPosition(out, blockPosition);

}

bool LecternUpdatePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLecternUpdate(*this);
}

}  // namespace bedrock_protocol
