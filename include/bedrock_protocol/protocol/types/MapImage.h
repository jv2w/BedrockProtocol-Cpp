/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MapImage.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

/**
 * The map texture dimension limits.
 *
 * Nothing serialises a 2D image any more: as of 1.26.40 ClientboundMapItemDataPacket carries a flat,
 * self-counting pixel list whose length is independent of the width and height it also carries
 * (gophertunnel v1.58.0 minecraft/protocol/packet/client_bound_map_item_data.go:76-83), so only the
 * limits themselves survive.
 */
class MapImage final {
public:
    //these limits are enforced in the protocol in 1.20.0
    static constexpr std::int32_t MAX_HEIGHT = 128;
    static constexpr std::int32_t MAX_WIDTH = 128;

    MapImage() = delete;
};

}  // namespace bedrock_protocol::types
