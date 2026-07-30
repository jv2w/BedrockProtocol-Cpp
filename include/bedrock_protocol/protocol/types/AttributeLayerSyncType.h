/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayerSyncType.php
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
 * @see AttributeLayerSyncPayload
 */
class AttributeLayerSyncType final {
public:
    AttributeLayerSyncType() = delete;

    static constexpr std::int32_t UPDATE_LAYERS = 0;
    static constexpr std::int32_t UPDATE_LAYER_SETTINGS = 1;
    static constexpr std::int32_t UPDATE_ENVIRONMENT = 2;
    static constexpr std::int32_t REMOVE_ENVIRONMENT = 3;
};

}  // namespace bedrock_protocol::types
