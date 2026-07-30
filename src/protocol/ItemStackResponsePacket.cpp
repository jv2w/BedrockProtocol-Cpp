/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ItemStackResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ItemStackResponsePacket.h"

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

ItemStackResponsePacket ItemStackResponsePacket::create(std::vector<types::inventory::stackresponse::ItemStackResponse> responses)
{
    ItemStackResponsePacket result;
    result.responses = std::move(responses);
    return result;
}

void ItemStackResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    responses.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        responses.push_back(types::inventory::stackresponse::ItemStackResponse::read(in));
    }

}

void ItemStackResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(responses.size()));
    for (const auto &response : responses) {
        response.write(out);
    }

}

bool ItemStackResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleItemStackResponse(*this);
}

}  // namespace bedrock_protocol
