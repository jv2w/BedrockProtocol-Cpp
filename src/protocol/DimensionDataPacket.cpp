/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/DimensionDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/DimensionDataPacket.h"

#include <algorithm>
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

DimensionDataPacket DimensionDataPacket::create(std::vector<std::pair<std::string, types::DimensionData>> definitions)
{
    DimensionDataPacket result;
    result.definitions = std::move(definitions);
    return result;
}

void DimensionDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    definitions.clear();

    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto dimensionNameId = serializer::CommonTypes::getString(in);
        const auto dimensionData = types::DimensionData::read(in);

        const auto existing = std::find_if(definitions.begin(), definitions.end(), [&dimensionNameId](const auto &entry) { return entry.first == dimensionNameId; });
        if (existing != definitions.end()) {
            throw PacketDecodeException("Repeated dimension data for key \"" + dimensionNameId + "\"");
        }
        if (dimensionNameId != types::DimensionNameIds::OVERWORLD && dimensionNameId != types::DimensionNameIds::NETHER && dimensionNameId != types::DimensionNameIds::THE_END) {
            throw PacketDecodeException("Invalid dimension name ID \"" + dimensionNameId + "\"");
        }
        definitions.emplace_back(dimensionNameId, dimensionData);
    }

}

void DimensionDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(definitions.size()));

    for (const auto &[dimensionNameId, definition] : definitions) {
        serializer::CommonTypes::putString(out, dimensionNameId); //@phpstan-ignore-line
        definition.write(out);
    }

}

bool DimensionDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleDimensionData(*this);
}

}  // namespace bedrock_protocol
