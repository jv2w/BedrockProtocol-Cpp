/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CodeBuilderPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CodeBuilderPacket.h"

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

CodeBuilderPacket CodeBuilderPacket::create(std::string url, bool openCodeBuilder)
{
    CodeBuilderPacket result;
    result.url = std::move(url);
    result.openCodeBuilder = openCodeBuilder;
    return result;
}

void CodeBuilderPacket::decodePayload(encoding::ByteBufferReader &in)
{
    url = serializer::CommonTypes::getString(in);
    openCodeBuilder = serializer::CommonTypes::getBool(in);

}

void CodeBuilderPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, url);
    serializer::CommonTypes::putBool(out, openCodeBuilder);

}

bool CodeBuilderPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCodeBuilder(*this);
}

}  // namespace bedrock_protocol
