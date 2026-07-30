/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ItemStackRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ItemStackRequestPacket.h"

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

ItemStackRequestPacket ItemStackRequestPacket::create(std::vector<types::inventory::stackrequest::ItemStackRequest> requests)
{
    ItemStackRequestPacket result;
    result.requests = std::move(requests);
    return result;
}

void ItemStackRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    requests.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        requests.push_back(types::inventory::stackrequest::ItemStackRequest::read(in));
    }

}

void ItemStackRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(requests.size()));
    for (const auto &request : requests) {
        request.write(out);
    }

}

bool ItemStackRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleItemStackRequest(*this);
}

}  // namespace bedrock_protocol
