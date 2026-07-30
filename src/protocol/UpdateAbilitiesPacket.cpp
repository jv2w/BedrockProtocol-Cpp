/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/UpdateAbilitiesPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateAbilitiesPacket.h"

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

UpdateAbilitiesPacket UpdateAbilitiesPacket::create(types::AbilitiesData data)
{
    UpdateAbilitiesPacket result;
    result.data = std::move(data);
    return result;
}

void UpdateAbilitiesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    data = types::AbilitiesData::decode(in);

}

void UpdateAbilitiesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    data->encode(out);

}

bool UpdateAbilitiesPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateAbilities(*this);
}

}  // namespace bedrock_protocol
