/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/MapInfoRequestPacketClientPixel.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/MapInfoRequestPacketClientPixel.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::LE;

MapInfoRequestPacketClientPixel MapInfoRequestPacketClientPixel::read(encoding::ByteBufferReader &in)
{
    const auto color = LE::readUnsignedInt(in);
    const auto index = LE::readUnsignedShort(in);

    const auto x = static_cast<std::uint16_t>(index % Y_INDEX_MULTIPLIER);
    const auto y = static_cast<std::uint16_t>(index / Y_INDEX_MULTIPLIER);

    return MapInfoRequestPacketClientPixel(::bedrock_protocol::color::Color::fromRGBA(color), x, y);
}

void MapInfoRequestPacketClientPixel::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, color.toRGBA());
    LE::writeUnsignedShort(out, static_cast<std::uint16_t>(x + (y * Y_INDEX_MULTIPLIER)));
}

}  // namespace bedrock_protocol::types
