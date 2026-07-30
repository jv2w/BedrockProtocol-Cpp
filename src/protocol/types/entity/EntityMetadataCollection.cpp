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

#include "bedrock_protocol/protocol/types/entity/EntityMetadataCollection.h"

#include <stdexcept>
#include <typeinfo>

#include "bedrock_protocol/protocol/types/entity/BlockPosMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/ByteMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/CompoundTagMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/EntityMetadataProperties.h"
#include "bedrock_protocol/protocol/types/entity/FloatMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/IntMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/LongMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/ShortMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/StringMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/Vec3MetadataProperty.h"

namespace bedrock_protocol::types::entity {

MetadataProperty *EntityMetadataCollection::get(std::int32_t key) const
{
    for (const auto &[k, v] : properties) {
        if (k == key) {
            return v.get();
        }
    }
    return nullptr;
}

void EntityMetadataCollection::put(PropertyList &list, std::int32_t key, std::unique_ptr<MetadataProperty> value)
{
    for (auto &[k, v] : list) {
        if (k == key) {
            v = std::move(value);
            return;
        }
    }
    list.emplace_back(key, std::move(value));
}

void EntityMetadataCollection::setByte(std::int32_t key, std::int64_t value, bool force)
{

    set(key, std::make_unique<ByteMetadataProperty>(value), force);
}

void EntityMetadataCollection::setShort(std::int32_t key, std::int64_t value, bool force)
{
    set(key, std::make_unique<ShortMetadataProperty>(value), force);
}

void EntityMetadataCollection::setInt(std::int32_t key, std::int64_t value, bool force)
{
    set(key, std::make_unique<IntMetadataProperty>(value), force);
}

void EntityMetadataCollection::setFloat(std::int32_t key, float value, bool force)
{
    set(key, std::make_unique<FloatMetadataProperty>(value), force);
}

void EntityMetadataCollection::setString(std::int32_t key, const std::string &value, bool force)
{
    set(key, std::make_unique<StringMetadataProperty>(value), force);
}

void EntityMetadataCollection::setCompoundTag(std::int32_t key, const CacheableNbt<nbt::tag::CompoundTag> &value,
                                              bool force)
{
    set(key, std::make_unique<CompoundTagMetadataProperty>(value), force);
}

void EntityMetadataCollection::setBlockPos(std::int32_t key, const std::optional<BlockPosition> &value, bool force)
{
    set(key, std::make_unique<BlockPosMetadataProperty>(value.has_value() ? *value : BlockPosition(0, 0, 0)), force);
}

void EntityMetadataCollection::setLong(std::int32_t key, std::int64_t value, bool force)
{
    set(key, std::make_unique<LongMetadataProperty>(value), force);
}

void EntityMetadataCollection::setVector3(std::int32_t key, const std::optional<math::Vector3> &value, bool force)
{
    set(key, std::make_unique<Vec3MetadataProperty>(value.has_value() ? *value : math::Vector3(0, 0, 0)), force);
}

void EntityMetadataCollection::set(std::int32_t key, std::unique_ptr<MetadataProperty> value, bool force)
{
    MetadataProperty *existing = get(key);
    // Bound to a reference first so that typeid's operand is a plain lvalue; dereferencing the
    // unique_ptr inline reads as a side-effecting expression to the compiler.
    const MetadataProperty &incoming = *value;
    if (!force && existing != nullptr && typeid(*existing) != typeid(incoming)) {
        throw std::invalid_argument(std::string("Can't overwrite property with mismatching types (have ") +
                                    typeid(*existing).name() + ")");
    }
    if (existing == nullptr || !existing->equals(*value)) {
        put(dirtyProperties, key, value->clone());
        put(properties, key, std::move(value));
    }
}

void EntityMetadataCollection::setAtomicBatch(PropertyList properties, bool force)
{
    bool anyDirty = false;
    if (!force) {
        for (const auto &[key, value] : properties) {
            MetadataProperty *existing = get(key);
            const MetadataProperty &incoming = *value;
            if (existing != nullptr && typeid(*existing) != typeid(incoming)) {
                throw std::invalid_argument(std::string("Can't overwrite ") + typeid(*existing).name() + " with " +
                                            typeid(incoming).name());
            }
        }
    }
    for (const auto &[key, value] : properties) {
        MetadataProperty *existing = get(key);
        if (existing == nullptr || !existing->equals(*value)) {
            anyDirty = true;
            break;
        }
    }
    if (anyDirty) {
        for (auto &[key, value] : properties) {
            put(dirtyProperties, key, value->clone());
            put(this->properties, key, std::move(value));
        }
    }
}

void EntityMetadataCollection::setGenericFlag(std::int32_t flagId, bool value)
{
    const std::int32_t propertyId =
        flagId >= 64 ? EntityMetadataProperties::FLAGS2 : EntityMetadataProperties::FLAGS;
    const std::int32_t realFlagId = flagId % 64;
    MetadataProperty *flagSetProp = get(propertyId);
    std::int64_t flagSet;
    if (flagSetProp == nullptr) {
        flagSet = 0;
    }
    else if (const auto *asLong = dynamic_cast<const LongMetadataProperty *>(flagSetProp); asLong != nullptr) {
        flagSet = asLong->getValue();
    }
    else {
        throw std::invalid_argument(std::string("Wrong type found for flags, want long, but have ") +
                                    typeid(*flagSetProp).name());
    }

    if (((flagSet >> realFlagId) & 1) != (value ? 1 : 0)) {
        flagSet ^= (static_cast<std::int64_t>(1) << realFlagId);
        setLong(propertyId, flagSet);
    }
}

void EntityMetadataCollection::setPlayerFlag(std::int32_t flagId, bool value)
{
    MetadataProperty *flagSetProp = get(EntityMetadataProperties::PLAYER_FLAGS);
    std::int64_t flagSet;
    if (flagSetProp == nullptr) {
        flagSet = 0;
    }
    else if (const auto *asByte = dynamic_cast<const ByteMetadataProperty *>(flagSetProp); asByte != nullptr) {
        flagSet = asByte->getValue();
    }
    else {
        throw std::invalid_argument(std::string("Wrong type found for flags, want byte, but have ") +
                                    typeid(*flagSetProp).name());
    }
    if (((flagSet >> flagId) & 1) != (value ? 1 : 0)) {
        flagSet ^= (static_cast<std::int64_t>(1) << flagId);
        setByte(EntityMetadataProperties::PLAYER_FLAGS, flagSet);
    }
}

void EntityMetadataCollection::markDirty(std::int32_t key)
{
    MetadataProperty *existing = get(key);
    if (existing != nullptr) {
        put(dirtyProperties, key, existing->clone());
    }
}

void EntityMetadataCollection::markAllDirty()
{
    dirtyProperties.clear();
    for (const auto &[key, value] : properties) {
        dirtyProperties.emplace_back(key, value->clone());
    }
}

}  // namespace bedrock_protocol::types::entity
