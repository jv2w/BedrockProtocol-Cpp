/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/UpdateAttribute.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/entity/AttributeModifier.h"

namespace bedrock_protocol::types::entity {

class UpdateAttribute final {
public:
    /**
     * @param modifiers
     */
    UpdateAttribute(std::string id, float min, float max, float current, float defaultMin, float defaultMax,
                    float defaultValue, std::vector<AttributeModifier> modifiers)
        : id(std::move(id)), min(min), max(max), current(current), defaultMin(defaultMin), defaultMax(defaultMax),
          defaultValue(defaultValue), modifiers(std::move(modifiers))
    {
    }

    [[nodiscard]] const std::string &getId() const { return id; }

    [[nodiscard]] float getMin() const { return min; }

    [[nodiscard]] float getMax() const { return max; }

    [[nodiscard]] float getCurrent() const { return current; }

    [[nodiscard]] float getDefaultMin() const { return defaultMin; }

    [[nodiscard]] float getDefaultMax() const { return defaultMax; }

    [[nodiscard]] float getDefault() const { return defaultValue; }

    [[nodiscard]] const std::vector<AttributeModifier> &getModifiers() const { return modifiers; }

    static UpdateAttribute read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string id;
    float min;
    float max;
    float current;
    float defaultMin;
    float defaultMax;
    /** Named `default` in the PHP original; renamed because `default` is a C++ keyword. */
    float defaultValue;
    std::vector<AttributeModifier> modifiers;
};

}  // namespace bedrock_protocol::types::entity
