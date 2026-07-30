/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/TrimDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/TrimDataPacket.h"

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

TrimDataPacket TrimDataPacket::create(std::vector<types::TrimPattern> trimPatterns, std::vector<types::TrimMaterial> trimMaterials)
{
    TrimDataPacket result;
    result.trimPatterns = std::move(trimPatterns);
    result.trimMaterials = std::move(trimMaterials);
    return result;
}

void TrimDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    trimPatterns.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        trimPatterns.push_back(types::TrimPattern::read(in));
    }
    trimMaterials.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        trimMaterials.push_back(types::TrimMaterial::read(in));
    }

}

void TrimDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(trimPatterns.size()));
    for (const auto &trimPattern : trimPatterns) {
        trimPattern.write(out);
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(trimMaterials.size()));
    for (const auto &trimMaterial : trimMaterials) {
        trimMaterial.write(out);
    }

}

bool TrimDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleTrimData(*this);
}

}  // namespace bedrock_protocol
