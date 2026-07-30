/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ScriptMessagePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ScriptMessagePacket.h"

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

ScriptMessagePacket ScriptMessagePacket::create(std::string messageId, std::string value)
{
    ScriptMessagePacket result;
    result.messageId = std::move(messageId);
    result.value = std::move(value);
    return result;
}

void ScriptMessagePacket::decodePayload(encoding::ByteBufferReader &in)
{
    messageId = serializer::CommonTypes::getString(in);
    value = serializer::CommonTypes::getString(in);

}

void ScriptMessagePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, messageId);
    serializer::CommonTypes::putString(out, value);

}

bool ScriptMessagePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleScriptMessage(*this);
}

}  // namespace bedrock_protocol
