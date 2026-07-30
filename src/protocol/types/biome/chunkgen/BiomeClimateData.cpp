/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeClimateData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeClimateData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeClimateData BiomeClimateData::read(encoding::ByteBufferReader &in)
{
    const auto temperature = encoding::LE::readFloat(in);
    const auto downfall = encoding::LE::readFloat(in);
    const auto snowAccumulationMin = encoding::LE::readFloat(in);
    const auto snowAccumulationMax = encoding::LE::readFloat(in);

    return BiomeClimateData(temperature, downfall, snowAccumulationMin, snowAccumulationMax);
}

void BiomeClimateData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, temperature);
    encoding::LE::writeFloat(out, downfall);
    encoding::LE::writeFloat(out, snowAccumulationMin);
    encoding::LE::writeFloat(out, snowAccumulationMax);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
