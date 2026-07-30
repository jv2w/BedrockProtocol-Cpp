/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CodeBuilderSourcePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CodeBuilderSourcePacket.h"

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

CodeBuilderSourcePacket CodeBuilderSourcePacket::create(std::uint8_t operation, std::uint8_t category, std::uint8_t codeStatus)
{
    CodeBuilderSourcePacket result;
    result.operation = operation;
    result.category = category;
    result.codeStatus = codeStatus;
    return result;
}

void CodeBuilderSourcePacket::decodePayload(encoding::ByteBufferReader &in)
{
    operation = encoding::Byte::readUnsigned(in);
    category = encoding::Byte::readUnsigned(in);
    codeStatus = encoding::Byte::readUnsigned(in);

}

void CodeBuilderSourcePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, operation);
    encoding::Byte::writeUnsigned(out, category);
    encoding::Byte::writeUnsigned(out, codeStatus);

}

bool CodeBuilderSourcePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCodeBuilderSource(*this);
}

}  // namespace bedrock_protocol
