/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/BlockPaletteEntry.php
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

#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"

namespace bedrock_protocol::types {

class BlockPaletteEntry final {
public:
    BlockPaletteEntry(std::string name, CacheableNbt<nbt::tag::CompoundTag> states)
        : name(std::move(name)), states(std::move(states))
    {
    }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const CacheableNbt<nbt::tag::CompoundTag> &getStates() const { return states; }

private:
    std::string name;
    CacheableNbt<nbt::tag::CompoundTag> states;
};

}  // namespace bedrock_protocol::types
