/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RequestPermissionsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RequestPermissionsPacket.h"

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

RequestPermissionsPacket RequestPermissionsPacket::create(std::int64_t targetActorUniqueId, std::int32_t playerPermission, std::uint16_t customFlags)
{
    RequestPermissionsPacket result;
    result.targetActorUniqueId = targetActorUniqueId;
    result.playerPermission = playerPermission;
    result.customFlags = customFlags;
    return result;
}

void RequestPermissionsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    targetActorUniqueId = encoding::LE::readSignedLong(in);
    playerPermission = encoding::VarInt::readSignedInt(in);
    customFlags = encoding::LE::readUnsignedShort(in);

}

void RequestPermissionsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedLong(out, targetActorUniqueId);
    encoding::VarInt::writeSignedInt(out, playerPermission);
    encoding::LE::writeUnsignedShort(out, customFlags);

}

bool RequestPermissionsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRequestPermissions(*this);
}

}  // namespace bedrock_protocol
