/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PacketViolationWarningPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PacketViolationWarningPacket.h"

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

PacketViolationWarningPacket PacketViolationWarningPacket::create(std::int32_t type, std::int32_t severity, std::int32_t packetId, std::string message)
{
    PacketViolationWarningPacket result;
    result.type = type;
    result.severity = severity;
    result.packetId = packetId;
    result.message = std::move(message);
    return result;
}

void PacketViolationWarningPacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::VarInt::readSignedInt(in);
    severity = encoding::VarInt::readSignedInt(in);
    packetId = encoding::VarInt::readSignedInt(in);
    message = serializer::CommonTypes::getString(in);

}

void PacketViolationWarningPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, type);
    encoding::VarInt::writeSignedInt(out, severity);
    encoding::VarInt::writeSignedInt(out, packetId);
    serializer::CommonTypes::putString(out, message);

}

bool PacketViolationWarningPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePacketViolationWarning(*this);
}

}  // namespace bedrock_protocol
