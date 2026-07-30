/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeNoiseGradientSurfaceData BiomeNoiseGradientSurfaceData::read(encoding::ByteBufferReader &in)
{
    std::vector<std::uint32_t> nonReplaceableBlocks;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        nonReplaceableBlocks.push_back(encoding::LE::readUnsignedInt(in));
    }

    std::vector<BiomeNoiseBlockSpecifier> gradientBlocks;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        gradientBlocks.push_back(BiomeNoiseBlockSpecifier::read(in));
    }

    auto noiseSeed = serializer::CommonTypes::getString(in);
    const auto firstOctave = encoding::LE::readUnsignedInt(in);

    std::vector<float> amplitudes;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        amplitudes.push_back(encoding::LE::readFloat(in));
    }

    return BiomeNoiseGradientSurfaceData(std::move(nonReplaceableBlocks), std::move(gradientBlocks),
                                         std::move(noiseSeed), firstOctave, std::move(amplitudes));
}

void BiomeNoiseGradientSurfaceData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(nonReplaceableBlocks.size()));
    for (const auto &value : nonReplaceableBlocks) {
        encoding::LE::writeUnsignedInt(out, value);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(gradientBlocks.size()));
    for (const auto &value : gradientBlocks) {
        value.write(out);
    }

    serializer::CommonTypes::putString(out, noiseSeed);
    encoding::LE::writeUnsignedInt(out, firstOctave);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(amplitudes.size()));
    for (const auto &value : amplitudes) {
        encoding::LE::writeFloat(out, value);
    }
}

}  // namespace bedrock_protocol::types::biome::chunkgen
