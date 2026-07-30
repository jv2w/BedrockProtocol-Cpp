/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/StructureTemplateDataRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/StructureTemplateDataRequestPacket.h"

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

StructureTemplateDataRequestPacket StructureTemplateDataRequestPacket::create(std::string structureTemplateName, types::BlockPosition structureBlockPosition, types::StructureSettings structureSettings, std::uint8_t requestType)
{
    StructureTemplateDataRequestPacket result;
    result.structureTemplateName = std::move(structureTemplateName);
    result.structureBlockPosition = std::move(structureBlockPosition);
    result.structureSettings = std::move(structureSettings);
    result.requestType = requestType;
    return result;
}

void StructureTemplateDataRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    structureTemplateName = serializer::CommonTypes::getString(in);
    structureBlockPosition = serializer::CommonTypes::getBlockPosition(in);
    structureSettings = serializer::CommonTypes::getStructureSettings(in);
    requestType = encoding::Byte::readUnsigned(in);

}

void StructureTemplateDataRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, structureTemplateName);
    serializer::CommonTypes::putBlockPosition(out, structureBlockPosition);
    serializer::CommonTypes::putStructureSettings(out, structureSettings);
    encoding::Byte::writeUnsigned(out, requestType);

}

bool StructureTemplateDataRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleStructureTemplateDataRequest(*this);
}

}  // namespace bedrock_protocol
