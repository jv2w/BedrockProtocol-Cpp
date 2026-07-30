/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/StructureBlockUpdatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/StructureBlockUpdatePacket.h"

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

StructureBlockUpdatePacket StructureBlockUpdatePacket::create(types::BlockPosition blockPosition, types::StructureEditorData structureEditorData, bool isPowered, bool waterlogged)
{
    StructureBlockUpdatePacket result;
    result.blockPosition = std::move(blockPosition);
    result.structureEditorData = std::move(structureEditorData);
    result.isPowered = isPowered;
    result.waterlogged = waterlogged;
    return result;
}

void StructureBlockUpdatePacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    structureEditorData = serializer::CommonTypes::getStructureEditorData(in);
    isPowered = serializer::CommonTypes::getBool(in);
    waterlogged = serializer::CommonTypes::getBool(in);

}

void StructureBlockUpdatePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    serializer::CommonTypes::putStructureEditorData(out, structureEditorData);
    serializer::CommonTypes::putBool(out, isPowered);
    serializer::CommonTypes::putBool(out, waterlogged);

}

bool StructureBlockUpdatePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleStructureBlockUpdate(*this);
}

}  // namespace bedrock_protocol
