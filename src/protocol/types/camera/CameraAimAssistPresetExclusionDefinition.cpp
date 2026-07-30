/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPresetExclusionDefinition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPresetExclusionDefinition.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::VarInt;
using serializer::CommonTypes;

CameraAimAssistPresetExclusionDefinition CameraAimAssistPresetExclusionDefinition::read(encoding::ByteBufferReader &in)
{
    std::vector<std::string> blocks;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        blocks.push_back(CommonTypes::getString(in));
    }

    std::vector<std::string> entities;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        entities.push_back(CommonTypes::getString(in));
    }

    std::vector<std::string> blockTags;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        blockTags.push_back(CommonTypes::getString(in));
    }

    std::vector<std::string> entityTypeFamilies;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        entityTypeFamilies.push_back(CommonTypes::getString(in));
    }

    return CameraAimAssistPresetExclusionDefinition(std::move(blocks), std::move(entities), std::move(blockTags),
                                                    std::move(entityTypeFamilies));
}

void CameraAimAssistPresetExclusionDefinition::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blocks.size()));
    for (const auto &block : blocks) {
        CommonTypes::putString(out, block);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entities.size()));
    for (const auto &entity : entities) {
        CommonTypes::putString(out, entity);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blockTags.size()));
    for (const auto &blockTag : blockTags) {
        CommonTypes::putString(out, blockTag);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entityTypeFamilies.size()));
    for (const auto &entityTypeFamily : entityTypeFamilies) {
        CommonTypes::putString(out, entityTypeFamily);
    }
}

}  // namespace bedrock_protocol::types::camera
