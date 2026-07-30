/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/Attribute.php
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
#include <vector>

#include "bedrock_protocol/protocol/types/entity/AttributeModifier.h"

namespace bedrock_protocol::types::entity {

class Attribute final {
public:
    /**
     * @param modifiers
     *
     * @note The members are assigned in the body rather than in a member initialiser list. <windows.h>
     *       defines min and max as function-like macros unless NOMINMAX is set, and `min(min)` in an
     *       initialiser list would be substituted away; a plain assignment is never followed by '(' and
     *       so is immune. This keeps the field names identical to the PHP original without an #undef
     *       leaking into every header included afterwards.
     */
    Attribute(std::string id, float min, float max, float current, float defaultValue,
              std::vector<AttributeModifier> modifiers)
    {
        this->id = std::move(id);
        this->min = min;
        this->max = max;
        this->current = current;
        this->defaultValue = defaultValue;
        this->modifiers = std::move(modifiers);
    }

    [[nodiscard]] const std::string &getId() const { return id; }

    [[nodiscard]] float getMin() const { return min; }

    [[nodiscard]] float getMax() const { return max; }

    [[nodiscard]] float getCurrent() const { return current; }

    [[nodiscard]] float getDefault() const { return defaultValue; }

    [[nodiscard]] const std::vector<AttributeModifier> &getModifiers() const { return modifiers; }

private:
    std::string id;
    float min;
    float max;
    float current;
    /** Named `default` in the PHP original; renamed because `default` is a C++ keyword. */
    float defaultValue;
    std::vector<AttributeModifier> modifiers;
};

}  // namespace bedrock_protocol::types::entity
