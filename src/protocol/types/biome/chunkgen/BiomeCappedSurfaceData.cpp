/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeCappedSurfaceData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::biome::chunkgen {

BiomeCappedSurfaceData BiomeCappedSurfaceData::read(encoding::ByteBufferReader &in)
{
    std::vector<std::uint32_t> floorBlocks;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        floorBlocks.push_back(encoding::LE::readUnsignedInt(in));
    }

    std::vector<std::uint32_t> ceilingBlocks;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        ceilingBlocks.push_back(encoding::LE::readUnsignedInt(in));
    }

    const auto seaBlock = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return encoding::LE::readUnsignedInt(in); });
    const auto foundationBlock = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return encoding::LE::readUnsignedInt(in); });
    const auto beachBlock = serializer::CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return encoding::LE::readUnsignedInt(in); });

    return BiomeCappedSurfaceData(std::move(floorBlocks), std::move(ceilingBlocks), seaBlock, foundationBlock,
                                  beachBlock);
}

void BiomeCappedSurfaceData::write(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(floorBlocks.size()));
    for (const auto &block : floorBlocks) {
        encoding::LE::writeUnsignedInt(out, block);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(ceilingBlocks.size()));
    for (const auto &block : ceilingBlocks) {
        encoding::LE::writeUnsignedInt(out, block);
    }

    serializer::CommonTypes::writeOptional(
        out, seaBlock,
        [](encoding::ByteBufferWriter &out, std::uint32_t v) { encoding::LE::writeUnsignedInt(out, v); });
    serializer::CommonTypes::writeOptional(
        out, foundationBlock,
        [](encoding::ByteBufferWriter &out, std::uint32_t v) { encoding::LE::writeUnsignedInt(out, v); });
    serializer::CommonTypes::writeOptional(
        out, beachBlock,
        [](encoding::ByteBufferWriter &out, std::uint32_t v) { encoding::LE::writeUnsignedInt(out, v); });
}

}  // namespace bedrock_protocol::types::biome::chunkgen
