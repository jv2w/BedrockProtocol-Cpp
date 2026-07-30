/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeValueColor.php
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
#include "bedrock_protocol/protocol/types/AttributeValueColorValue.h"
#include "bedrock_protocol/protocol/types/AttributeValueType.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeEnvironment
 */
class AttributeValueColor final : public AttributeValue {
public:
    static constexpr std::int32_t ID = AttributeValueType::COLOR;

    static constexpr std::string_view OPERATION_OVERRIDE = "override";
    static constexpr std::string_view OPERATION_ALPHA_BLEND = "alpha_blend";
    static constexpr std::string_view OPERATION_ADD = "add";
    static constexpr std::string_view OPERATION_SUBTRACT = "subtract";
    static constexpr std::string_view OPERATION_MULTIPLY = "multiply";

    AttributeValueColor(std::unique_ptr<AttributeValueColorValue> value, std::string operation)
        : value(std::move(value)), operation(std::move(operation))
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const AttributeValueColorValue &getValue() const { return *value; }

    [[nodiscard]] const std::string &getOperation() const { return operation; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeValueColor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<AttributeValue> clone() const override
    {
        return std::make_unique<AttributeValueColor>(value->clone(), operation);
    }

private:
    std::unique_ptr<AttributeValueColorValue> value;
    std::string operation;
};

}  // namespace bedrock_protocol::types
