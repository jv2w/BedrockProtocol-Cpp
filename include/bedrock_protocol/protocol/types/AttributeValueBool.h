/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeValueBool.php
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
class AttributeValueBool final : public AttributeValue {
public:
    static constexpr std::int32_t ID = AttributeValueType::BOOL;

    static constexpr std::string_view OPERATION_OVERRIDE = "override";
    static constexpr std::string_view OPERATION_ALPHA_BLEND = "alpha_blend";
    static constexpr std::string_view OPERATION_AND = "and";
    static constexpr std::string_view OPERATION_NAND = "nand";
    static constexpr std::string_view OPERATION_OR = "or";
    static constexpr std::string_view OPERATION_NOR = "nor";
    static constexpr std::string_view OPERATION_XOR = "xor";
    static constexpr std::string_view OPERATION_XNOR = "xnor";

    AttributeValueBool(bool value, std::string operation) : value(value), operation(std::move(operation)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] bool getValue() const { return value; }

    [[nodiscard]] const std::string &getOperation() const { return operation; }

    static AttributeValueBool read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<AttributeValue> clone() const override
    {
        return std::make_unique<AttributeValueBool>(*this);
    }

private:
    bool value;
    std::string operation;
};

}  // namespace bedrock_protocol::types
