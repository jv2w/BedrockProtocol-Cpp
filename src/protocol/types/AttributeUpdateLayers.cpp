/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeUpdateLayers.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeUpdateLayers.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

AttributeUpdateLayers AttributeUpdateLayers::read(encoding::ByteBufferReader &in)
{
    std::vector<AttributeLayer> layers;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        layers.push_back(AttributeLayer::read(in));
    }

    return AttributeUpdateLayers(std::move(layers));
}

void AttributeUpdateLayers::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(layers.size()));
    for (const auto &layer : layers) {
        layer.write(out);
    }
}

}  // namespace bedrock_protocol::types
