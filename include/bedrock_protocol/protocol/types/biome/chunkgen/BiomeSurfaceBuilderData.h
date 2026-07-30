/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeSurfaceBuilderData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.h"
#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.h"

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeSurfaceBuilderData final {
public:
    BiomeSurfaceBuilderData(std::optional<BiomeSurfaceMaterialData> surfaceMaterial, bool defaultOverworldSurface,
                            bool swampSurface, bool frozenOceanSurface, bool theEndSurface,
                            std::optional<BiomeMesaSurfaceData> mesaSurface,
                            std::optional<BiomeCappedSurfaceData> cappedSurface,
                            std::optional<BiomeNoiseGradientSurfaceData> noiseGradientSurface)
        : surfaceMaterial(std::move(surfaceMaterial)),
          defaultOverworldSurface(defaultOverworldSurface),
          swampSurface(swampSurface),
          frozenOceanSurface(frozenOceanSurface),
          theEndSurface(theEndSurface),
          mesaSurface(std::move(mesaSurface)),
          cappedSurface(std::move(cappedSurface)),
          noiseGradientSurface(std::move(noiseGradientSurface))
    {
    }

    [[nodiscard]] const std::optional<BiomeSurfaceMaterialData> &getSurfaceMaterial() const { return surfaceMaterial; }

    [[nodiscard]] bool hasDefaultOverworldSurface() const { return defaultOverworldSurface; }

    [[nodiscard]] bool hasSwampSurface() const { return swampSurface; }

    [[nodiscard]] bool hasFrozenOceanSurface() const { return frozenOceanSurface; }

    [[nodiscard]] bool hasTheEndSurface() const { return theEndSurface; }

    [[nodiscard]] const std::optional<BiomeMesaSurfaceData> &getMesaSurface() const { return mesaSurface; }

    [[nodiscard]] const std::optional<BiomeCappedSurfaceData> &getCappedSurface() const { return cappedSurface; }

    [[nodiscard]] const std::optional<BiomeNoiseGradientSurfaceData> &getNoiseGradientSurface() const
    {
        return noiseGradientSurface;
    }

    /** @throws DataDecodeException */
    static BiomeSurfaceBuilderData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<BiomeSurfaceMaterialData> surfaceMaterial;
    bool defaultOverworldSurface;
    bool swampSurface;
    bool frozenOceanSurface;
    bool theEndSurface;
    std::optional<BiomeMesaSurfaceData> mesaSurface;
    std::optional<BiomeCappedSurfaceData> cappedSurface;
    std::optional<BiomeNoiseGradientSurfaceData> noiseGradientSurface;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
