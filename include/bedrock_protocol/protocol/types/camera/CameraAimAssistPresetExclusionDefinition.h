/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPresetExclusionDefinition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::camera {

class CameraAimAssistPresetExclusionDefinition final {
public:
    CameraAimAssistPresetExclusionDefinition(std::vector<std::string> blocks, std::vector<std::string> entities,
                                             std::vector<std::string> blockTags,
                                             std::vector<std::string> entityTypeFamilies)
        : blocks(std::move(blocks)), entities(std::move(entities)), blockTags(std::move(blockTags)),
          entityTypeFamilies(std::move(entityTypeFamilies))
    {
    }

    [[nodiscard]] const std::vector<std::string> &getBlocks() const { return blocks; }

    [[nodiscard]] const std::vector<std::string> &getEntities() const { return entities; }

    [[nodiscard]] const std::vector<std::string> &getBlockTags() const { return blockTags; }

    [[nodiscard]] const std::vector<std::string> &getEntityTypeFamilies() const { return entityTypeFamilies; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistPresetExclusionDefinition read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<std::string> blocks;
    std::vector<std::string> entities;
    std::vector<std::string> blockTags;
    std::vector<std::string> entityTypeFamilies;
};

}  // namespace bedrock_protocol::types::camera
