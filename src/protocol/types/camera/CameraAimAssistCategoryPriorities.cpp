/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistCategoryPriorities.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategoryPriorities.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

CameraAimAssistCategoryPriorities CameraAimAssistCategoryPriorities::read(encoding::ByteBufferReader &in)
{
    std::vector<CameraAimAssistCategoryPriority> entities;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        entities.push_back(CameraAimAssistCategoryPriority::read(in));
    }

    std::vector<CameraAimAssistCategoryPriority> blocks;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        blocks.push_back(CameraAimAssistCategoryPriority::read(in));
    }

    std::vector<CameraAimAssistCategoryPriority> blockTags;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        blockTags.push_back(CameraAimAssistCategoryPriority::read(in));
    }

    std::vector<CameraAimAssistCategoryPriority> entityTypeFamilies;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        entityTypeFamilies.push_back(CameraAimAssistCategoryPriority::read(in));
    }

    const auto defaultEntityPriority =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readSignedInt(in); });
    const auto defaultBlockPriority =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readSignedInt(in); });
    return CameraAimAssistCategoryPriorities(std::move(entities), std::move(blocks), std::move(blockTags),
                                             std::move(entityTypeFamilies), defaultEntityPriority,
                                             defaultBlockPriority);
}

void CameraAimAssistCategoryPriorities::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entities.size()));
    for (const auto &entity : entities) {
        entity.write(out);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blocks.size()));
    for (const auto &block : blocks) {
        block.write(out);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blockTags.size()));
    for (const auto &tag : blockTags) {
        tag.write(out);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entityTypeFamilies.size()));
    for (const auto &family : entityTypeFamilies) {
        family.write(out);
    }

    CommonTypes::writeOptional(out, defaultEntityPriority,
                               [](encoding::ByteBufferWriter &out, std::int32_t v) { LE::writeSignedInt(out, v); });
    CommonTypes::writeOptional(out, defaultBlockPriority,
                               [](encoding::ByteBufferWriter &out, std::int32_t v) { LE::writeSignedInt(out, v); });
}

}  // namespace bedrock_protocol::types::camera
