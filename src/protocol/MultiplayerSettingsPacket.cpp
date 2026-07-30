/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MultiplayerSettingsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MultiplayerSettingsPacket.h"

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

MultiplayerSettingsPacket MultiplayerSettingsPacket::create(std::int32_t action)
{
    MultiplayerSettingsPacket result;
    result.action = action;
    return result;
}

void MultiplayerSettingsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    action = encoding::VarInt::readSignedInt(in);

}

void MultiplayerSettingsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, action);

}

bool MultiplayerSettingsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMultiplayerSettings(*this);
}

}  // namespace bedrock_protocol
