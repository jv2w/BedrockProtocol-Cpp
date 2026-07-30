/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryWithCacheList.php
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

#include "bedrock_protocol/protocol/types/SubChunkPacketEntryWithCache.h"

namespace bedrock_protocol::types {

class SubChunkPacketEntryWithCacheList final {
public:
    explicit SubChunkPacketEntryWithCacheList(std::vector<SubChunkPacketEntryWithCache> entries) : entries(std::move(entries)) {}

    [[nodiscard]] const std::vector<SubChunkPacketEntryWithCache> &getEntries() const { return entries; }

private:
    std::vector<SubChunkPacketEntryWithCache> entries;
};

}  // namespace bedrock_protocol::types
