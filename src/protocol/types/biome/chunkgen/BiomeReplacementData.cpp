/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeReplacementData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeReplacementData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeReplacementData BiomeReplacementData::read(encoding::ByteBufferReader &in)
{
    const auto biome = encoding::LE::readSignedShort(in);
    const auto dimension = encoding::VarInt::readSignedInt(in);
    std::vector<std::int16_t> targetBiomes;
    const auto targetBiomeCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < targetBiomeCount; ++i) {
        targetBiomes.push_back(encoding::LE::readSignedShort(in));
    }
    const auto amount = encoding::LE::readFloat(in);
    const auto replacementIndex = encoding::LE::readUnsignedInt(in);
    return BiomeReplacementData(biome, dimension, std::move(targetBiomes), amount, replacementIndex);
}

void BiomeReplacementData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedShort(out, biome);
    encoding::VarInt::writeSignedInt(out, dimension);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(targetBiomes.size()));
    for (const auto &biome : targetBiomes) {
        encoding::LE::writeSignedShort(out, biome);
    }
    encoding::LE::writeFloat(out, amount);
    encoding::LE::writeUnsignedInt(out, replacementIndex);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
