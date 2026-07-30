/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeValueColorArray.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeValueColorType.h"
#include "bedrock_protocol/protocol/types/AttributeValueColorValue.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeValueColor
 */
class AttributeValueColorArray final : public AttributeValueColorValue {
public:
    static constexpr std::int32_t ID = AttributeValueColorType::ARRAY;

    explicit AttributeValueColorArray(const color::Color &value) : value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const color::Color &getValue() const { return value; }

    static AttributeValueColorArray read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<AttributeValueColorValue> clone() const override
    {
        return std::make_unique<AttributeValueColorArray>(*this);
    }

private:
    color::Color value;
};

}  // namespace bedrock_protocol::types
