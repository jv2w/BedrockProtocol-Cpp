/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeNoiseBlockSpecifier.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeNoiseBlockSpecifier BiomeNoiseBlockSpecifier::read(encoding::ByteBufferReader &in)
{
    auto noise = serializer::CommonTypes::getString(in);
    const auto threshold = encoding::LE::readFloat(in);
    const auto min = encoding::LE::readFloat(in);
    const auto max = encoding::LE::readFloat(in);
    const auto block = encoding::LE::readUnsignedInt(in);

    return BiomeNoiseBlockSpecifier(std::move(noise), threshold, min, max, block);
}

void BiomeNoiseBlockSpecifier::write(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, noise);
    encoding::LE::writeFloat(out, threshold);
    encoding::LE::writeFloat(out, min);
    encoding::LE::writeFloat(out, max);
    encoding::LE::writeUnsignedInt(out, block);
}

}  // namespace bedrock_protocol::types::biome::chunkgen
