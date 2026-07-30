/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ShowCreditsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ShowCreditsPacket.h"

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

ShowCreditsPacket ShowCreditsPacket::create(std::uint64_t playerActorRuntimeId, std::int32_t status)
{
    ShowCreditsPacket result;
    result.playerActorRuntimeId = playerActorRuntimeId;
    result.status = status;
    return result;
}

void ShowCreditsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    playerActorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    status = encoding::VarInt::readSignedInt(in);

}

void ShowCreditsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, playerActorRuntimeId);
    encoding::VarInt::writeSignedInt(out, status);

}

bool ShowCreditsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleShowCredits(*this);
}

}  // namespace bedrock_protocol
