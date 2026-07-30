/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerPresenceInfoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerPresenceInfoPacket.h"

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

ServerPresenceInfoPacket ServerPresenceInfoPacket::create(std::optional<types::PresenceInfo> presenceConfig)
{
    ServerPresenceInfoPacket result;
    result.presenceConfig = std::move(presenceConfig);
    return result;
}

void ServerPresenceInfoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    presenceConfig = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::PresenceInfo::read(reader); });

}

void ServerPresenceInfoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(out, presenceConfig, [](encoding::ByteBufferWriter &out, const types::PresenceInfo &v) { v.write(out); });

}

bool ServerPresenceInfoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerPresenceInfo(*this);
}

}  // namespace bedrock_protocol
