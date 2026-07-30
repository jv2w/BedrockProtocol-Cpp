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

#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategoryPriority.h"

namespace bedrock_protocol::types::camera {

class CameraAimAssistCategoryPriorities final {
public:
    CameraAimAssistCategoryPriorities(std::vector<CameraAimAssistCategoryPriority> entities,
                                      std::vector<CameraAimAssistCategoryPriority> blocks,
                                      std::vector<CameraAimAssistCategoryPriority> blockTags,
                                      std::vector<CameraAimAssistCategoryPriority> entityTypeFamilies,
                                      std::optional<std::int32_t> defaultEntityPriority,
                                      std::optional<std::int32_t> defaultBlockPriority)
        : entities(std::move(entities)), blocks(std::move(blocks)), blockTags(std::move(blockTags)),
          entityTypeFamilies(std::move(entityTypeFamilies)), defaultEntityPriority(defaultEntityPriority),
          defaultBlockPriority(defaultBlockPriority)
    {
    }

    [[nodiscard]] const std::vector<CameraAimAssistCategoryPriority> &getEntities() const { return entities; }

    [[nodiscard]] const std::vector<CameraAimAssistCategoryPriority> &getBlocks() const { return blocks; }

    [[nodiscard]] const std::vector<CameraAimAssistCategoryPriority> &getBlockTags() const { return blockTags; }

    [[nodiscard]] const std::vector<CameraAimAssistCategoryPriority> &getEntityTypeFamilies() const
    {
        return entityTypeFamilies;
    }

    [[nodiscard]] const std::optional<std::int32_t> &getDefaultEntityPriority() const { return defaultEntityPriority; }

    [[nodiscard]] const std::optional<std::int32_t> &getDefaultBlockPriority() const { return defaultBlockPriority; }

    /**
     * @throws DataDecodeException
     */
    static CameraAimAssistCategoryPriorities read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<CameraAimAssistCategoryPriority> entities;
    std::vector<CameraAimAssistCategoryPriority> blocks;
    std::vector<CameraAimAssistCategoryPriority> blockTags;
    std::vector<CameraAimAssistCategoryPriority> entityTypeFamilies;
    std::optional<std::int32_t> defaultEntityPriority;
    std::optional<std::int32_t> defaultBlockPriority;
};

}  // namespace bedrock_protocol::types::camera
