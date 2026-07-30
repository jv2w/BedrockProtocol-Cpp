/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/LegacyTelemetryEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LegacyTelemetryEventPacket.h"

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

void LegacyTelemetryEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    playerRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    eventData = encoding::VarInt::readSignedInt(in);
    type = encoding::Byte::readUnsigned(in);

    //TODO: nice confusing mess

}

void LegacyTelemetryEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, playerRuntimeId);
    encoding::VarInt::writeSignedInt(out, eventData);
    encoding::Byte::writeUnsigned(out, type);

    //TODO: also nice confusing mess

}

bool LegacyTelemetryEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLegacyTelemetryEvent(*this);
}

}  // namespace bedrock_protocol
