/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/AttributeModifierOperation.php
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
 * Affects how the modifier value is applied to the target attribute. These operations are described on the Minecraft
 * Wiki at https://minecraft.fandom.com/wiki/Attribute
 *
 * These operations are listed in the order that they are applied in.
 */
class AttributeModifierOperation final {
    AttributeModifierOperation() = delete;

public:
    /** Adds the modifier value to the attribute's base value. */
    static constexpr std::int32_t ADD = 0;
    /**
     * Multiplies the value by (1 + x), where x is the sum of all MULTIPLY_BASE modifiers' amounts. Multiple modifiers
     * of this type have additive effects on each other.
     */
    static constexpr std::int32_t MULTIPLY_BASE = 1;
    /**
     * Each modifier of this type multiplies the value by (1 + x), where x is the current modifier's value. Multiple
     * modifiers of this type have multiplicative effects on each other.
     */
    static constexpr std::int32_t MULTIPLY_TOTAL = 2;
    /**
     * Limits the result value. If the result value is greater than the limit, it is set to the limit.
     */
    static constexpr std::int32_t CAP = 3;
};

}  // namespace bedrock_protocol::types::entity
