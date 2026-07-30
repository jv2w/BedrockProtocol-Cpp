/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeOverworldGenRulesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeOverworldGenRulesData BiomeOverworldGenRulesData::read(encoding::ByteBufferReader &in)
{
    std::vector<BiomeWeightedData> hillTransformations;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        hillTransformations.push_back(BiomeWeightedData::read(in));
    }

    std::vector<BiomeWeightedData> mutateTransformations;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        mutateTransformations.push_back(BiomeWeightedData::read(in));
    }

    std::vector<BiomeWeightedData> riverTransformations;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        riverTransformations.push_back(BiomeWeightedData::read(in));
    }

    std::vector<BiomeWeightedData> shoreTransformations;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        shoreTransformations.push_back(BiomeWeightedData::read(in));
    }

    std::vector<BiomeConditionalTransformationData> preHillsEdges;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        preHillsEdges.push_back(BiomeConditionalTransformationData::read(in));
    }

    std::vector<BiomeConditionalTransformationData> postShoreEdges;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        postShoreEdges.push_back(BiomeConditionalTransformationData::read(in));
    }

    std::vector<BiomeWeightedTemperatureData> climates;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        climates.push_back(BiomeWeightedTemperatureData::read(in));
    }

    return BiomeOverworldGenRulesData(std::move(hillTransformations), std::move(mutateTransformations),
                                      std::move(riverTransformations), std::move(shoreTransformations),
                                      std::move(preHillsEdges), std::move(postShoreEdges), std::move(climates));
}

void BiomeOverworldGenRulesData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(hillTransformations.size()));
    for (const auto &transformation : hillTransformations) {
        transformation.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(mutateTransformations.size()));
    for (const auto &transformation : mutateTransformations) {
        transformation.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(riverTransformations.size()));
    for (const auto &transformation : riverTransformations) {
        transformation.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(shoreTransformations.size()));
    for (const auto &transformation : shoreTransformations) {
        transformation.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(preHillsEdges.size()));
    for (const auto &edge : preHillsEdges) {
        edge.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(postShoreEdges.size()));
    for (const auto &edge : postShoreEdges) {
        edge.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(climates.size()));
    for (const auto &climate : climates) {
        climate.write(out);
    }
}

}  // namespace bedrock_protocol::types::biome::chunkgen
