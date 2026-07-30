/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SettingsCommandPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SettingsCommandPacket.h"

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

SettingsCommandPacket SettingsCommandPacket::create(std::string command, bool suppressOutput)
{
    SettingsCommandPacket result;
    result.command = std::move(command);
    result.suppressOutput = suppressOutput;
    return result;
}

void SettingsCommandPacket::decodePayload(encoding::ByteBufferReader &in)
{
    command = serializer::CommonTypes::getString(in);
    suppressOutput = serializer::CommonTypes::getBool(in);

}

void SettingsCommandPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, command);
    serializer::CommonTypes::putBool(out, suppressOutput);

}

bool SettingsCommandPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSettingsCommand(*this);
}

}  // namespace bedrock_protocol
