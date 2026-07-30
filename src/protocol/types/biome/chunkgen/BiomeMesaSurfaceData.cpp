/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeMesaSurfaceData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeMesaSurfaceData BiomeMesaSurfaceData::read(encoding::ByteBufferReader &in)
{
    const auto clayMaterial = encoding::LE::readUnsignedInt(in);
    const auto hardClayMaterial = encoding::LE::readUnsignedInt(in);
    const auto brycePillars = serializer::CommonTypes::getBool(in);
    const auto forest = serializer::CommonTypes::getBool(in);

    return BiomeMesaSurfaceData(clayMaterial, hardClayMaterial, brycePillars, forest);
}

void BiomeMesaSurfaceData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedInt(out, clayMaterial);
    encoding::LE::writeUnsignedInt(out, hardClayMaterial);
    serializer::CommonTypes::putBool(out, brycePillars);
    serializer::CommonTypes::putBool(out, forest);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
