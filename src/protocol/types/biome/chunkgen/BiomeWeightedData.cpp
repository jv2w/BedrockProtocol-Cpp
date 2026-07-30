/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeWeightedData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeWeightedData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeWeightedData BiomeWeightedData::read(encoding::ByteBufferReader &in)
{
    const auto biome = encoding::LE::readSignedShort(in);
    const auto weight = encoding::LE::readUnsignedInt(in);

    return BiomeWeightedData(biome, weight);
}

void BiomeWeightedData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedShort(out, biome);
    encoding::LE::writeUnsignedInt(out, weight);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
