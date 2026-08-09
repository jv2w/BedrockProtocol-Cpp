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
    // The type string alone decides whether a body follows; there is no presence flag on the wire.
    // Reading one consumed a byte that belongs to the marker data.
    type = serializer::CommonTypes::getString(in);
    data = type == TYPE_ADD_CUBE ? std::optional(types::DebugMarkerData::read(in)) : std::nullopt;
}

void ClientboundDebugRendererPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, type);
    if (type == TYPE_ADD_CUBE && data.has_value()) {
        data->write(out);
    }
}

bool ClientboundDebugRendererPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundDebugRenderer(*this);
}

}  // namespace bedrock_protocol
