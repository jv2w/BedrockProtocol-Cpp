/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeWeightedTemperatureData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeWeightedTemperatureData BiomeWeightedTemperatureData::read(encoding::ByteBufferReader &in)
{
    const auto temperature = encoding::VarInt::readSignedInt(in);
    const auto weight = encoding::LE::readUnsignedInt(in);

    return BiomeWeightedTemperatureData(temperature, weight);
}

void BiomeWeightedTemperatureData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, temperature);
    encoding::LE::writeUnsignedInt(out, weight);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
