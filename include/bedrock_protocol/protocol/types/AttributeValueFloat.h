/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeValueFloat.php
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
#include <string>
#include <string_view>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeValue.h"
#include "bedrock_protocol/protocol/types/AttributeValueType.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeEnvironment
 */
class AttributeValueFloat final : public AttributeValue {
public:
    static constexpr std::int32_t ID = AttributeValueType::FLOAT;

    static constexpr std::string_view OPERATION_OVERRIDE = "override";
    static constexpr std::string_view OPERATION_ALPHA_BLEND = "alpha_blend";
    static constexpr std::string_view OPERATION_ADD = "add";
    static constexpr std::string_view OPERATION_SUBTRACT = "subtract";
    static constexpr std::string_view OPERATION_MULTIPLY = "multiply";
    static constexpr std::string_view OPERATION_MINIMUM = "minimum";
    static constexpr std::string_view OPERATION_MAXIMUM = "maximum";

    AttributeValueFloat(float value, std::string operation) : value(value), operation(std::move(operation)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] const std::string &getOperation() const { return operation; }

    static AttributeValueFloat read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] std::unique_ptr<AttributeValue> clone() const override
    {
        return std::make_unique<AttributeValueFloat>(*this);
    }

private:
    float value;
    std::string operation;
};

}  // namespace bedrock_protocol::types
