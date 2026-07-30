/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerFogPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerFogPacket.h"

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

PlayerFogPacket PlayerFogPacket::create(std::vector<std::string> fogLayers)
{
    PlayerFogPacket result;
    result.fogLayers = std::move(fogLayers);
    return result;
}

void PlayerFogPacket::decodePayload(encoding::ByteBufferReader &in)
{
    fogLayers.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        fogLayers.push_back(serializer::CommonTypes::getString(in));
    }

}

void PlayerFogPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(fogLayers.size()));
    for (const auto &fogLayer : fogLayers) {
        serializer::CommonTypes::putString(out, fogLayer);
    }

}

bool PlayerFogPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerFog(*this);
}

}  // namespace bedrock_protocol
