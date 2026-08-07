/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MapInfoRequestPacketClientPixel.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/MapInfoRequestPacketClientPixel.h"

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::BE;
using encoding::LE;

MapInfoRequestPacketClientPixel MapInfoRequestPacketClientPixel::read(encoding::ByteBufferReader &in)
{
    //RGBA, not BEARGB: gophertunnel v1.58.0 minecraft/protocol/reader.go:175-184 unpacks a LITTLE-endian
    //uint32 as R | G<<8 | B<<16 | A<<24, i.e. the bytes R, G, B, A - which is a big-endian read of
    //an RGBA-packed word.
    const auto color = BE::readUnsignedInt(in);
    const auto index = LE::readUnsignedShort(in);

    const auto x = static_cast<std::uint16_t>(index % Y_INDEX_MULTIPLIER);
    const auto y = static_cast<std::uint16_t>(index / Y_INDEX_MULTIPLIER);

    return MapInfoRequestPacketClientPixel(::bedrock_protocol::color::Color::fromRGBA(color), x, y);
}

void MapInfoRequestPacketClientPixel::write(encoding::ByteBufferWriter &out) const
{
    BE::writeUnsignedInt(out, color.toRGBA());
    LE::writeUnsignedShort(out, static_cast<std::uint16_t>(x + (y * Y_INDEX_MULTIPLIER)));
}

}  // namespace bedrock_protocol::types
