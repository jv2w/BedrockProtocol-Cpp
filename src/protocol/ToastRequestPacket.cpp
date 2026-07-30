/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ToastRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ToastRequestPacket.h"

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

ToastRequestPacket ToastRequestPacket::create(std::string title, std::string body)
{
    ToastRequestPacket result;
    result.title = std::move(title);
    result.body = std::move(body);
    return result;
}

void ToastRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    title = serializer::CommonTypes::getString(in);
    body = serializer::CommonTypes::getString(in);

}

void ToastRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, title);
    serializer::CommonTypes::putString(out, body);

}

bool ToastRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleToastRequest(*this);
}

}  // namespace bedrock_protocol
