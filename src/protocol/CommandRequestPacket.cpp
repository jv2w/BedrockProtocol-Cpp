/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CommandRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CommandRequestPacket.h"

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

CommandRequestPacket CommandRequestPacket::create(std::string command, types::command::CommandOriginData originData, bool isInternal, std::string version)
{
    CommandRequestPacket result;
    result.command = std::move(command);
    result.originData = std::move(originData);
    result.isInternal = isInternal;
    result.version = std::move(version);
    return result;
}

void CommandRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    command = serializer::CommonTypes::getString(in);
    originData = serializer::CommonTypes::getCommandOriginData(in);
    isInternal = serializer::CommonTypes::getBool(in);
    version = serializer::CommonTypes::getString(in);

}

void CommandRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, command);
    serializer::CommonTypes::putCommandOriginData(out, originData);
    serializer::CommonTypes::putBool(out, isInternal);
    serializer::CommonTypes::putString(out, version);

}

bool CommandRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCommandRequest(*this);
}

}  // namespace bedrock_protocol
