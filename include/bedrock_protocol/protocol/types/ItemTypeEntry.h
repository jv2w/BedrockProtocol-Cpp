/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ItemTypeEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"

namespace bedrock_protocol::types {

class ItemTypeEntry final {
public:
    ItemTypeEntry(std::string stringId, std::int16_t numericId, bool componentBased, std::int32_t version,
                  CacheableNbt<nbt::tag::CompoundTag> componentNbt)
        : stringId(std::move(stringId)), numericId(numericId), componentBased(componentBased), version(version),
          componentNbt(std::move(componentNbt))
    {
    }

    [[nodiscard]] const std::string &getStringId() const { return stringId; }

    [[nodiscard]] std::int16_t getNumericId() const { return numericId; }

    [[nodiscard]] bool isComponentBased() const { return componentBased; }

    [[nodiscard]] std::int32_t getVersion() const { return version; }

    [[nodiscard]] const CacheableNbt<nbt::tag::CompoundTag> &getComponentNbt() const { return componentNbt; }

private:
    std::string stringId;
    std::int16_t numericId;
    bool componentBased;
    std::int32_t version;
    CacheableNbt<nbt::tag::CompoundTag> componentNbt;
};

}  // namespace bedrock_protocol::types
