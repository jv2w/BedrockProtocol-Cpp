/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePackClientResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackClientResponsePacket.h"

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

ResourcePackClientResponsePacket ResourcePackClientResponsePacket::create(std::uint8_t status, std::vector<std::string> packIds)
{
    ResourcePackClientResponsePacket result;
    result.status = status;
    result.packIds = std::move(packIds);
    return result;
}

void ResourcePackClientResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    status = encoding::Byte::readUnsigned(in);
    auto entryCount = encoding::LE::readUnsignedShort(in);
    packIds.clear();
    while (entryCount-- > 0) {
        packIds.push_back(serializer::CommonTypes::getString(in));
    }

}

void ResourcePackClientResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, status);
    encoding::LE::writeUnsignedShort(out, static_cast<std::uint16_t>(packIds.size()));
    for (const auto &id : packIds) {
        serializer::CommonTypes::putString(out, id);
    }

}

bool ResourcePackClientResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackClientResponse(*this);
}

}  // namespace bedrock_protocol
