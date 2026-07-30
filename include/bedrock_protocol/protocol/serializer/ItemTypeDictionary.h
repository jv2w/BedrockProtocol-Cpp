/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/serializer/ItemTypeDictionary.php
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
#include <string_view>
#include <unordered_map>
#include <vector>

#include "bedrock_protocol/protocol/types/ItemTypeEntry.h"

namespace bedrock_protocol::serializer {

class ItemTypeDictionary final {
public:
    /**
     * @param itemTypes
     */
    explicit ItemTypeDictionary(std::vector<types::ItemTypeEntry> itemTypes);

    /**
     * @return
     */
    [[nodiscard]] const std::vector<types::ItemTypeEntry> &getEntries() const
    {
        return itemTypes;
    }

    /**
     * @throws std::invalid_argument
     */
    [[nodiscard]] std::int16_t fromStringId(const std::string &stringId) const;

    /**
     * @throws std::invalid_argument
     */
    [[nodiscard]] const std::string &fromIntId(std::int16_t intId) const;

private:
    /**
     * @note lookup only; iteration order is never observed
     */
    std::unordered_map<std::int16_t, std::string> intToStringIdMap;
    /**
     * @note lookup only; iteration order is never observed
     */
    std::unordered_map<std::string, std::int16_t> stringToIntMap;

    std::vector<types::ItemTypeEntry> itemTypes;
};

}  // namespace bedrock_protocol::serializer
