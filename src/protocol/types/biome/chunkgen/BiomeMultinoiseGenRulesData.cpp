/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeMultinoiseGenRulesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeMultinoiseGenRulesData BiomeMultinoiseGenRulesData::read(encoding::ByteBufferReader &in)
{
    const auto temperature = encoding::LE::readFloat(in);
    const auto humidity = encoding::LE::readFloat(in);
    const auto altitude = encoding::LE::readFloat(in);
    const auto weirdness = encoding::LE::readFloat(in);
    const auto weight = encoding::LE::readFloat(in);

    return BiomeMultinoiseGenRulesData(temperature, humidity, altitude, weirdness, weight);
}

void BiomeMultinoiseGenRulesData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, temperature);
    encoding::LE::writeFloat(out, humidity);
    encoding::LE::writeFloat(out, altitude);
    encoding::LE::writeFloat(out, weirdness);
    encoding::LE::writeFloat(out, weight);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
