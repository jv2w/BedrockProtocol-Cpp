/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundDebugRendererPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundDebugRendererPacket.h"

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

ClientboundDebugRendererPacket ClientboundDebugRendererPacket::base(std::string type)
{
    ClientboundDebugRendererPacket result;
    result.type = std::move(type);
    return result;
}

ClientboundDebugRendererPacket ClientboundDebugRendererPacket::clear() { return base(std::string(TYPE_CLEAR)); }

ClientboundDebugRendererPacket ClientboundDebugRendererPacket::addCube(types::DebugMarkerData data)
{
    ClientboundDebugRendererPacket result = base(std::string(TYPE_ADD_CUBE));
    result.data = std::move(data);
    return result;
}

void ClientboundDebugRendererPacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = serializer::CommonTypes::getString(in);
    data = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::DebugMarkerData::read(reader); });

}

void ClientboundDebugRendererPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, type);
    serializer::CommonTypes::writeOptional(out, data, [](encoding::ByteBufferWriter &out, const types::DebugMarkerData &data) { data.write(out); });

}

bool ClientboundDebugRendererPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundDebugRenderer(*this);
}

}  // namespace bedrock_protocol
