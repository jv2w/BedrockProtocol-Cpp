/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeConsolidatedFeatureData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeConsolidatedFeatureData BiomeConsolidatedFeatureData::read(encoding::ByteBufferReader &in)
{
    auto scatter = BiomeScatterParamData::read(in);
    const auto feature = encoding::LE::readSignedShort(in);
    const auto identifier = encoding::LE::readSignedShort(in);
    const auto pass = encoding::LE::readSignedShort(in);
    const auto useInternal = serializer::CommonTypes::getBool(in);

    return BiomeConsolidatedFeatureData(std::move(scatter), feature, identifier, pass, useInternal);
}

void BiomeConsolidatedFeatureData::write(encoding::ByteBufferWriter &out) const
{
    scatter.write(out);
    encoding::LE::writeSignedShort(out, feature);
    encoding::LE::writeSignedShort(out, identifier);
    encoding::LE::writeSignedShort(out, pass);
    serializer::CommonTypes::putBool(out, useInternal);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
