/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BiomeDefinitionListPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BiomeDefinitionListPacket.h"

#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

BiomeDefinitionListPacket BiomeDefinitionListPacket::create(std::vector<types::biome::BiomeDefinitionData> definitionData, std::vector<std::string> strings)
{
    BiomeDefinitionListPacket result;
    result.definitionData = std::move(definitionData);
    result.strings = std::move(strings);
    return result;
}

BiomeDefinitionListPacket BiomeDefinitionListPacket::fromDefinitions(
    const std::vector<types::biome::BiomeDefinitionEntry> &definitions)
{
    // The wire format stores every biome name and tag as an index into one shared string table, so
    // each distinct string is added once and reused.
    std::vector<std::string> strings;
    std::unordered_map<std::string, std::uint16_t> stringIndexLookup;
    const auto addString = [&strings, &stringIndexLookup](const std::string &string) -> std::uint16_t {
        const auto existing = stringIndexLookup.find(string);
        if (existing != stringIndexLookup.end()) {
            return existing->second;
        }
        const auto index = static_cast<std::uint16_t>(stringIndexLookup.size());
        stringIndexLookup.emplace(string, index);
        strings.push_back(string);
        return index;
    };

    std::vector<types::biome::BiomeDefinitionData> definitionData;
    definitionData.reserve(definitions.size());
    for (const auto &entry : definitions) {
        const auto nameIndex = addString(entry.getBiomeName());

        std::optional<std::vector<std::uint16_t>> tagIndexes;
        if (entry.getTags().has_value()) {
            std::vector<std::uint16_t> indexes;
            indexes.reserve(entry.getTags()->size());
            for (const auto &tag : *entry.getTags()) {
                indexes.push_back(addString(tag));
            }
            tagIndexes = std::move(indexes);
        }

        definitionData.emplace_back(nameIndex, entry.getId(), entry.getTemperature(), entry.getDownfall(),
                                    entry.getFoliageSnow(), entry.getDepth(), entry.getScale(),
                                    entry.getMapWaterColor(), entry.hasRain(), std::move(tagIndexes),
                                    entry.getChunkGenData());
    }

    return create(std::move(definitionData), std::move(strings));
}

void BiomeDefinitionListPacket::decodePayload(encoding::ByteBufferReader &in)
{
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        definitionData.push_back(types::biome::BiomeDefinitionData::read(in));
    }

    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        strings.push_back(serializer::CommonTypes::getString(in));
    }

}

void BiomeDefinitionListPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(definitionData.size()));
    for (const auto &data : definitionData) {
        data.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(strings.size()));
    for (const auto &string : strings) {
        serializer::CommonTypes::putString(out, string);
    }

}

bool BiomeDefinitionListPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBiomeDefinitionList(*this);
}

}  // namespace bedrock_protocol
