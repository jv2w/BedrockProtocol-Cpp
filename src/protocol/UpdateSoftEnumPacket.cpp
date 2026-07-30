/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateSoftEnumPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateSoftEnumPacket.h"

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

UpdateSoftEnumPacket UpdateSoftEnumPacket::create(std::string enumName, std::vector<std::string> values, std::uint8_t type)
{
    UpdateSoftEnumPacket result;
    result.enumName = std::move(enumName);
    result.values = std::move(values);
    result.type = type;
    return result;
}

void UpdateSoftEnumPacket::decodePayload(encoding::ByteBufferReader &in)
{
    enumName = serializer::CommonTypes::getString(in);
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        values.push_back(serializer::CommonTypes::getString(in));
    }
    type = encoding::Byte::readUnsigned(in);

}

void UpdateSoftEnumPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, enumName);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(values.size()));
    for (const auto &v : values) {
        serializer::CommonTypes::putString(out, v);
    }
    encoding::Byte::writeUnsigned(out, type);

}

bool UpdateSoftEnumPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateSoftEnum(*this);
}

}  // namespace bedrock_protocol
