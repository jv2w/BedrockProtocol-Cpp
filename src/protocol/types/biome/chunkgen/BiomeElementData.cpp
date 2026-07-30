/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeElementData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeElementData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeElementData BiomeElementData::read(encoding::ByteBufferReader &in)
{
    const auto noiseFrequencyScale = encoding::LE::readFloat(in);
    const auto noiseLowerBound = encoding::LE::readFloat(in);
    const auto noiseUpperBound = encoding::LE::readFloat(in);
    const auto heightMinType = encoding::VarInt::readSignedInt(in);
    const auto heightMin = encoding::LE::readSignedShort(in);
    const auto heightMaxType = encoding::VarInt::readSignedInt(in);
    const auto heightMax = encoding::LE::readSignedShort(in);
    auto surfaceMaterial = BiomeSurfaceMaterialData::read(in);

    return BiomeElementData(noiseFrequencyScale, noiseLowerBound, noiseUpperBound, heightMinType, heightMin,
                            heightMaxType, heightMax, std::move(surfaceMaterial));
}

void BiomeElementData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, noiseFrequencyScale);
    encoding::LE::writeFloat(out, noiseLowerBound);
    encoding::LE::writeFloat(out, noiseUpperBound);
    encoding::VarInt::writeSignedInt(out, heightMinType);
    encoding::LE::writeSignedShort(out, heightMin);
    encoding::VarInt::writeSignedInt(out, heightMaxType);
    encoding::LE::writeSignedShort(out, heightMax);
    surfaceMaterial.write(out);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
