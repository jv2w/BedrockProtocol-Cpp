/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryWithoutCacheList.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/types/SubChunkPacketEntryWithoutCache.h"

namespace bedrock_protocol::types {

class SubChunkPacketEntryWithoutCacheList final {
public:
    explicit SubChunkPacketEntryWithoutCacheList(std::vector<SubChunkPacketEntryWithoutCache> entries) : entries(std::move(entries)) {}

    [[nodiscard]] const std::vector<SubChunkPacketEntryWithoutCache> &getEntries() const { return entries; }

private:
    std::vector<SubChunkPacketEntryWithoutCache> entries;
};

}  // namespace bedrock_protocol::types
