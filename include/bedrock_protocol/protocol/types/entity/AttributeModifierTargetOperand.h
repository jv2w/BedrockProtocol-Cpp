/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/AttributeModifierTargetOperand.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::entity {

/**
 * Affects which parameter of the target attribute is modified.
 */
class AttributeModifierTargetOperand final {
    AttributeModifierTargetOperand() = delete;

public:
    static constexpr std::int32_t MIN = 0;
    static constexpr std::int32_t MAX = 1;
    static constexpr std::int32_t CURRENT = 2;
};

}  // namespace bedrock_protocol::types::entity
