/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/BiomeDefinitionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/BiomeDefinitionData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome {

BiomeDefinitionData BiomeDefinitionData::read(encoding::ByteBufferReader &in)
{
    const auto nameIndex = encoding::LE::readUnsignedShort(in);
    const auto id = encoding::LE::readUnsignedShort(in);
    const auto temperature = encoding::LE::readFloat(in);
    const auto downfall = encoding::LE::readFloat(in);
    const auto foliageSnow = encoding::LE::readFloat(in);
    const auto depth = encoding::LE::readFloat(in);
    const auto scale = encoding::LE::readFloat(in);
    const auto mapWaterColor = color::Color::fromARGB(encoding::LE::readUnsignedInt(in));
    const auto rain = serializer::CommonTypes::getBool(in);
    auto tags = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        std::vector<std::uint16_t> tagIndexes;

        for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
            tagIndexes.push_back(encoding::LE::readUnsignedShort(in));
        }

        return tagIndexes;
    });
    auto chunkGenData = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return chunkgen::BiomeDefinitionChunkGenData::read(in); });

    return BiomeDefinitionData(nameIndex, id, temperature, downfall, foliageSnow, depth, scale, mapWaterColor, rain,
                               std::move(tags), std::move(chunkGenData));
}

void BiomeDefinitionData::write(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedShort(out, nameIndex);
    encoding::LE::writeUnsignedShort(out, id);
    encoding::LE::writeFloat(out, temperature);
    encoding::LE::writeFloat(out, downfall);
    encoding::LE::writeFloat(out, foliageSnow);
    encoding::LE::writeFloat(out, depth);
    encoding::LE::writeFloat(out, scale);
    encoding::LE::writeUnsignedInt(out, mapWaterColor.toARGB());
    serializer::CommonTypes::putBool(out, rain);
    serializer::CommonTypes::writeOptional(
        out, tagIndexes, [](encoding::ByteBufferWriter &out, const std::vector<std::uint16_t> &tagIndexes) {
            encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(tagIndexes.size()));
            for (const auto &tag : tagIndexes) {
                encoding::LE::writeUnsignedShort(out, tag);
            }
        });
    serializer::CommonTypes::writeOptional(
        out, chunkGenData,
        [](encoding::ByteBufferWriter &out, const chunkgen::BiomeDefinitionChunkGenData &v) { v.write(out); });
}

}  // namespace bedrock_protocol::types::biome
