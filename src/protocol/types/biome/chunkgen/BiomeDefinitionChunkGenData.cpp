/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeDefinitionChunkGenData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeDefinitionChunkGenData.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeDefinitionChunkGenData BiomeDefinitionChunkGenData::read(encoding::ByteBufferReader &in)
{
    auto climate = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeClimateData::read(in); });
    auto consolidatedFeatures = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeConsolidatedFeaturesData::read(in); });
    auto mountainParams = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeMountainParamsData::read(in); });
    auto surfaceMaterialAdjustment = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeSurfaceMaterialAdjustmentData::read(in); });
    auto overworldGenRules = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeOverworldGenRulesData::read(in); });
    auto multinoiseGenRules = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeMultinoiseGenRulesData::read(in); });
    auto legacyWorldGenRules = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeLegacyWorldGenRulesData::read(in); });
    auto replacementsData =
        serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) -> std::vector<BiomeReplacementData> {
            const auto count = encoding::VarInt::readUnsignedInt(in);
            std::vector<BiomeReplacementData> result;
            for (std::uint32_t i = 0; i < count; ++i) {
                result.push_back(BiomeReplacementData::read(in));
            }
            return result;
        });
    const auto villageType = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return encoding::Byte::readUnsigned(in); });
    auto surfaceBuilderData = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeSurfaceBuilderData::read(in); });
    auto subSurfaceBuilderData = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return BiomeSurfaceBuilderData::read(in); });

    return BiomeDefinitionChunkGenData(std::move(climate), std::move(consolidatedFeatures), std::move(mountainParams),
                                       std::move(surfaceMaterialAdjustment), std::move(overworldGenRules),
                                       std::move(multinoiseGenRules), std::move(legacyWorldGenRules),
                                       std::move(replacementsData), villageType, std::move(surfaceBuilderData),
                                       std::move(subSurfaceBuilderData));
}

void BiomeDefinitionChunkGenData::write(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(
        out, climate, [](encoding::ByteBufferWriter &out, const BiomeClimateData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, consolidatedFeatures,
        [](encoding::ByteBufferWriter &out, const BiomeConsolidatedFeaturesData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, mountainParams, [](encoding::ByteBufferWriter &out, const BiomeMountainParamsData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, surfaceMaterialAdjustment,
        [](encoding::ByteBufferWriter &out, const BiomeSurfaceMaterialAdjustmentData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, overworldGenRules,
        [](encoding::ByteBufferWriter &out, const BiomeOverworldGenRulesData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, multinoiseGenRules,
        [](encoding::ByteBufferWriter &out, const BiomeMultinoiseGenRulesData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, legacyWorldGenRules,
        [](encoding::ByteBufferWriter &out, const BiomeLegacyWorldGenRulesData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, replacementsData, [](encoding::ByteBufferWriter &out, const std::vector<BiomeReplacementData> &v) {
            encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(v.size()));
            for (const auto &item : v) {
                item.write(out);
            }
        });
    serializer::CommonTypes::writeOptional(
        out, villageType,
        [](encoding::ByteBufferWriter &out, std::uint8_t v) { encoding::Byte::writeUnsigned(out, v); });
    serializer::CommonTypes::writeOptional(
        out, surfaceBuilderData,
        [](encoding::ByteBufferWriter &out, const BiomeSurfaceBuilderData &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(
        out, subSurfaceBuilderData,
        [](encoding::ByteBufferWriter &out, const BiomeSurfaceBuilderData &v) { v.write(out); });
}

}  // namespace bedrock_protocol::types::biome::chunkgen
