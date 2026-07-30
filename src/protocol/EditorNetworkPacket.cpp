/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/EditorNetworkPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/EditorNetworkPacket.h"

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

EditorNetworkPacket EditorNetworkPacket::create(bool isRouteToManager, types::CacheableNbt<nbt::tag::CompoundTag> payload)
{
    EditorNetworkPacket result;
    result.isRouteToManager = isRouteToManager;
    result.payload = std::move(payload);
    return result;
}

void EditorNetworkPacket::decodePayload(encoding::ByteBufferReader &in)
{
    isRouteToManager = serializer::CommonTypes::getBool(in);
    payload = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void EditorNetworkPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, isRouteToManager);
    out.writeByteArray(payload.getEncodedNbt());

}

bool EditorNetworkPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleEditorNetwork(*this);
}

}  // namespace bedrock_protocol
