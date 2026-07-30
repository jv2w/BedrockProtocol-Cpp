/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayStatusPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayStatusPacket.h"

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

PlayStatusPacket PlayStatusPacket::create(std::uint32_t status)
{
    PlayStatusPacket result;
    result.status = status;
    return result;
}

void PlayStatusPacket::decodePayload(encoding::ByteBufferReader &in)
{
    status = encoding::BE::readUnsignedInt(in);

}

void PlayStatusPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::BE::writeUnsignedInt(out, status);

}

bool PlayStatusPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayStatus(*this);
}

}  // namespace bedrock_protocol
