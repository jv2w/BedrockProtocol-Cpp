/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CameraPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraPacket.h"

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

CameraPacket CameraPacket::create(std::int64_t cameraActorUniqueId, std::int64_t playerActorUniqueId)
{
    CameraPacket result;
    result.cameraActorUniqueId = cameraActorUniqueId;
    result.playerActorUniqueId = playerActorUniqueId;
    return result;
}

void CameraPacket::decodePayload(encoding::ByteBufferReader &in)
{
    cameraActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    playerActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);

}

void CameraPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, cameraActorUniqueId);
    serializer::CommonTypes::putActorUniqueId(out, playerActorUniqueId);

}

bool CameraPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCamera(*this);
}

}  // namespace bedrock_protocol
