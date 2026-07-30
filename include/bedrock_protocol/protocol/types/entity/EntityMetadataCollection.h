/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/EntityMetadataCollection.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

namespace bedrock_protocol::types::entity {

class EntityMetadataCollection {
public:
    /**
     * @note The PHP original stores properties in an insertion-ordered array<int, MetadataProperty>; the
     *       encode order depends on that iteration order, so ordered key/value pairs are used here.
     */
    using PropertyList = std::vector<std::pair<std::int32_t, std::unique_ptr<MetadataProperty>>>;

    EntityMetadataCollection() = default;

    void setByte(std::int32_t key, std::int64_t value, bool force = false);

    void setShort(std::int32_t key, std::int64_t value, bool force = false);

    void setInt(std::int32_t key, std::int64_t value, bool force = false);

    void setFloat(std::int32_t key, float value, bool force = false);

    void setString(std::int32_t key, const std::string &value, bool force = false);

    void setCompoundTag(std::int32_t key, const CacheableNbt<nbt::tag::CompoundTag> &value, bool force = false);

    void setBlockPos(std::int32_t key, const std::optional<BlockPosition> &value, bool force = false);

    void setLong(std::int32_t key, std::int64_t value, bool force = false);

    void setVector3(std::int32_t key, const std::optional<math::Vector3> &value, bool force = false);

    void set(std::int32_t key, std::unique_ptr<MetadataProperty> value, bool force = false);

    /**
     * Set a group of properties together. If any of them are changed, they will all be flagged as dirty.
     *
     * @param properties
     */
    void setAtomicBatch(PropertyList properties, bool force = false);

    void setGenericFlag(std::int32_t flagId, bool value);

    void setPlayerFlag(std::int32_t flagId, bool value);

    /**
     * Returns all properties.
     */
    [[nodiscard]] const PropertyList &getAll() const { return properties; }

    /**
     * Returns properties that have changed and need to be broadcasted.
     */
    [[nodiscard]] const PropertyList &getDirty() const { return dirtyProperties; }

    /**
     * Clears records of dirty properties.
     */
    void clearDirtyProperties() { dirtyProperties.clear(); }

    /**
     * Marks the selected property for resending.
     */
    void markDirty(std::int32_t key);

    /**
     * Marks all properties for resending.
     */
    void markAllDirty();

private:
    /** Returns the entry for the given key, or nullptr if the key is not present. */
    [[nodiscard]] MetadataProperty *get(std::int32_t key) const;

    /** Inserts or replaces the given key in an insertion-ordered list. */
    static void put(PropertyList &list, std::int32_t key, std::unique_ptr<MetadataProperty> value);

    PropertyList properties;
    PropertyList dirtyProperties;
};

}  // namespace bedrock_protocol::types::entity
