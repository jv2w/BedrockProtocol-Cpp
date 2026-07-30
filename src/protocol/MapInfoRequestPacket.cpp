/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MapInfoRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MapInfoRequestPacket.h"

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

MapInfoRequestPacket MapInfoRequestPacket::create(std::int64_t mapId, std::vector<types::MapInfoRequestPacketClientPixel> clientPixels)
{
    MapInfoRequestPacket result;
    result.mapId = mapId;
    result.clientPixels = std::move(clientPixels);
    return result;
}

void MapInfoRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    mapId = serializer::CommonTypes::getActorUniqueId(in);

    clientPixels.clear();
    const auto count = encoding::LE::readUnsignedInt(in);
    if (count > types::MapImage::MAX_HEIGHT * types::MapImage::MAX_WIDTH) {
        throw PacketDecodeException("Too many pixels");
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        clientPixels.push_back(types::MapInfoRequestPacketClientPixel::read(in));
    }

}

void MapInfoRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, mapId);

    encoding::LE::writeUnsignedInt(out, static_cast<std::uint32_t>(clientPixels.size()));
    for (const auto &pixel : clientPixels) {
        pixel.write(out);
    }

}

bool MapInfoRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMapInfoRequest(*this);
}

}  // namespace bedrock_protocol
