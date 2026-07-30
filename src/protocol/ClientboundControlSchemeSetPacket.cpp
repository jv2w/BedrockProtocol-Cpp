/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundControlSchemeSetPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundControlSchemeSetPacket.h"

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

ClientboundControlSchemeSetPacket ClientboundControlSchemeSetPacket::create(types::ControlScheme scheme)
{
    ClientboundControlSchemeSetPacket result;
    result.scheme = scheme;
    return result;
}

void ClientboundControlSchemeSetPacket::decodePayload(encoding::ByteBufferReader &in)
{
    scheme = types::ControlSchemeFromPacket(encoding::Byte::readUnsigned(in));

}

void ClientboundControlSchemeSetPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(scheme));

}

bool ClientboundControlSchemeSetPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundControlSchemeSet(*this);
}

}  // namespace bedrock_protocol
