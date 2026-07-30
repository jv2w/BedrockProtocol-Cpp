/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateBlockSyncedPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateBlockSyncedPacket.h"

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

void UpdateBlockSyncedPacket::decodePayload(encoding::ByteBufferReader &in)
{
    UpdateBlockPacket::decodePayload(in);
    actorUniqueId = encoding::VarInt::readUnsignedLong(in);
    updateType = encoding::VarInt::readUnsignedLong(in);

}

void UpdateBlockSyncedPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    UpdateBlockPacket::encodePayload(out);
    encoding::VarInt::writeUnsignedLong(out, actorUniqueId);
    encoding::VarInt::writeUnsignedLong(out, updateType);

}

bool UpdateBlockSyncedPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateBlockSynced(*this);
}

}  // namespace bedrock_protocol
