/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeSurfaceMaterialData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeSurfaceMaterialData BiomeSurfaceMaterialData::read(encoding::ByteBufferReader &in)
{
    const auto topBlock = encoding::LE::readUnsignedInt(in);
    const auto midBlock = encoding::LE::readUnsignedInt(in);
    const auto seaFloorBlock = encoding::LE::readUnsignedInt(in);
    const auto foundationBlock = encoding::LE::readUnsignedInt(in);
    const auto seaBlock = encoding::LE::readUnsignedInt(in);
    const auto seaFloorDepth = encoding::LE::readSignedInt(in);

    return BiomeSurfaceMaterialData(topBlock, midBlock, seaFloorBlock, foundationBlock, seaBlock, seaFloorDepth);
}

void BiomeSurfaceMaterialData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedInt(out, topBlock);
    encoding::LE::writeUnsignedInt(out, midBlock);
    encoding::LE::writeUnsignedInt(out, seaFloorBlock);
    encoding::LE::writeUnsignedInt(out, foundationBlock);
    encoding::LE::writeUnsignedInt(out, seaBlock);
    encoding::LE::writeSignedInt(out, seaFloorDepth);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
