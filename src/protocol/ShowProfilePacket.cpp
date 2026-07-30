/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ShowProfilePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ShowProfilePacket.h"

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

ShowProfilePacket ShowProfilePacket::create(std::string xuid)
{
    ShowProfilePacket result;
    result.xuid = std::move(xuid);
    return result;
}

void ShowProfilePacket::decodePayload(encoding::ByteBufferReader &in)
{
    xuid = serializer::CommonTypes::getString(in);

}

void ShowProfilePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, xuid);

}

bool ShowProfilePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleShowProfile(*this);
}

}  // namespace bedrock_protocol
