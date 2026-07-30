/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeSurfaceBuilderData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceBuilderData.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeSurfaceBuilderData BiomeSurfaceBuilderData::read(encoding::ByteBufferReader &in)
{
    auto surfaceMaterial = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeSurfaceMaterialData::read(in); });
    const auto defaultOverworldSurface = serializer::CommonTypes::getBool(in);
    const auto swampSurface = serializer::CommonTypes::getBool(in);
    const auto frozenOceanSurface = serializer::CommonTypes::getBool(in);
    const auto theEndSurface = serializer::CommonTypes::getBool(in);
    auto mesaSurface = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeMesaSurfaceData::read(in); });
    auto cappedSurface = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeCappedSurfaceData::read(in); });
    auto noiseGradientSurface = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeNoiseGradientSurfaceData::read(in); });

    return BiomeSurfaceBuilderData(std::move(surfaceMaterial), defaultOverworldSurface, swampSurface,
                                   frozenOceanSurface, theEndSurface, std::move(mesaSurface), std::move(cappedSurface),
                                   std::move(noiseGradientSurface));
}

void BiomeSurfaceBuilderData::write(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(
        out, surfaceMaterial,
        [](encoding::ByteBufferWriter &out, const BiomeSurfaceMaterialData &v) { v.write(out); });
    serializer::CommonTypes::putBool(out, defaultOverworldSurface);
    serializer::CommonTypes::putBool(out, swampSurface);
    serializer::CommonTypes::putBool(out, frozenOceanSurface);
    serializer::CommonTypes::putBool(out, theEndSurface);
    serializer::CommonTypes::writeOptional(
        out, mesaSurface, [](encoding::ByteBufferWriter &out, const BiomeMesaSurfaceData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, cappedSurface, [](encoding::ByteBufferWriter &out, const BiomeCappedSurfaceData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, noiseGradientSurface,
        [](encoding::ByteBufferWriter &out, const BiomeNoiseGradientSurfaceData &v) { v.write(out); });
}

}  // namespace bedrock_protocol::types::biome::chunkgen
