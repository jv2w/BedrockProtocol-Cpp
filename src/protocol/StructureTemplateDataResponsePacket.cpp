/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/StructureTemplateDataResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/StructureTemplateDataResponsePacket.h"

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

StructureTemplateDataResponsePacket StructureTemplateDataResponsePacket::create(std::string structureTemplateName, std::optional<types::CacheableNbt<nbt::tag::CompoundTag>> nbt, std::uint8_t responseType)
{
    StructureTemplateDataResponsePacket result;
    result.structureTemplateName = std::move(structureTemplateName);
    result.nbt = std::move(nbt);
    result.responseType = responseType;
    return result;
}

void StructureTemplateDataResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    structureTemplateName = serializer::CommonTypes::getString(in);
    if (serializer::CommonTypes::getBool(in)) {
        nbt = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));
    }
    responseType = encoding::Byte::readUnsigned(in);

}

void StructureTemplateDataResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, structureTemplateName);
    serializer::CommonTypes::putBool(out, nbt.has_value());
    if (nbt.has_value()) {
        out.writeByteArray(nbt->getEncodedNbt());
    }
    encoding::Byte::writeUnsigned(out, responseType);

}

bool StructureTemplateDataResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleStructureTemplateDataResponse(*this);
}

}  // namespace bedrock_protocol
