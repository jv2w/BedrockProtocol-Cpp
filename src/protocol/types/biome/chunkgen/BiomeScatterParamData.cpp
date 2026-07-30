/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeScatterParamData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeScatterParamData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeScatterParamData BiomeScatterParamData::read(encoding::ByteBufferReader &in)
{
    std::vector<BiomeCoordinateData> coordinates;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        coordinates.push_back(BiomeCoordinateData::read(in));
    }
    const auto evalOrder = encoding::VarInt::readSignedInt(in);
    const auto chancePercentType = encoding::VarInt::readSignedInt(in);
    const auto chancePercent = encoding::LE::readSignedShort(in);
    const auto chanceNumerator = encoding::LE::readSignedInt(in);
    const auto chanceDenominator = encoding::LE::readSignedInt(in);
    const auto iterationsType = encoding::VarInt::readSignedInt(in);
    const auto iterations = encoding::LE::readSignedShort(in);

    return BiomeScatterParamData(std::move(coordinates), evalOrder, chancePercentType, chancePercent, chanceNumerator,
                                 chanceDenominator, iterationsType, iterations);
}

void BiomeScatterParamData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(coordinates.size()));
    for (const auto &coordinate : coordinates) {
        coordinate.write(out);
    }
    encoding::VarInt::writeSignedInt(out, evalOrder);
    encoding::VarInt::writeSignedInt(out, chancePercentType);
    encoding::LE::writeSignedShort(out, chancePercent);
    encoding::LE::writeSignedInt(out, chanceNumerator);
    encoding::LE::writeSignedInt(out, chanceDenominator);
    encoding::VarInt::writeSignedInt(out, iterationsType);
    encoding::LE::writeSignedShort(out, iterations);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
