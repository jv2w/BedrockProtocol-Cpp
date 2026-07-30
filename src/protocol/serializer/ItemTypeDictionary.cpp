/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/ItemTypeDictionary.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/serializer/ItemTypeDictionary.h"

#include <stdexcept>
#include <utility>

namespace bedrock_protocol::serializer {

ItemTypeDictionary::ItemTypeDictionary(std::vector<types::ItemTypeEntry> itemTypes) : itemTypes(std::move(itemTypes))
{
    for (const auto &type : this->itemTypes) {
        stringToIntMap[type.getStringId()] = type.getNumericId();
        intToStringIdMap[type.getNumericId()] = type.getStringId();
    }
}

std::int16_t ItemTypeDictionary::fromStringId(const std::string &stringId) const
{
    const auto it = stringToIntMap.find(stringId);
    if (it == stringToIntMap.end()) {
        throw std::invalid_argument("Unmapped string ID \"" + stringId + "\"");
    }
    return it->second;
}

const std::string &ItemTypeDictionary::fromIntId(std::int16_t intId) const
{
    const auto it = intToStringIdMap.find(intId);
    if (it == intToStringIdMap.end()) {
        throw std::invalid_argument("Unmapped int ID " + std::to_string(intId));
    }
    return it->second;
}

}  // namespace bedrock_protocol::serializer
