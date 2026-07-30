/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RemoveVolumeEntityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RemoveVolumeEntityPacket.h"

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

RemoveVolumeEntityPacket RemoveVolumeEntityPacket::create(std::uint32_t entityNetId, std::int32_t dimension)
{
    RemoveVolumeEntityPacket result;
    result.entityNetId = entityNetId;
    result.dimension = dimension;
    return result;
}

void RemoveVolumeEntityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    entityNetId = encoding::VarInt::readUnsignedInt(in);
    dimension = encoding::VarInt::readSignedInt(in);

}

void RemoveVolumeEntityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, entityNetId);
    encoding::VarInt::writeSignedInt(out, dimension);

}

bool RemoveVolumeEntityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRemoveVolumeEntity(*this);
}

}  // namespace bedrock_protocol
