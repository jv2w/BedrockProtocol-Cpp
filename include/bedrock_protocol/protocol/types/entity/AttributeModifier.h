/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/AttributeModifier.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::entity {

class AttributeModifier final {
public:
    /**
     * @see AttributeModifierOperation
     * @see AttributeModifierTargetOperand
     */
    AttributeModifier(std::string id, std::string name, float amount, std::int32_t operation, std::int32_t operand,
                      bool serializable)  //???
        : id(std::move(id)), name(std::move(name)), amount(amount), operation(operation), operand(operand),
          serializable(serializable)
    {
    }

    [[nodiscard]] const std::string &getId() const { return id; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] float getAmount() const { return amount; }

    [[nodiscard]] std::int32_t getOperation() const { return operation; }

    [[nodiscard]] std::int32_t getOperand() const { return operand; }

    [[nodiscard]] bool isSerializable() const { return serializable; }

    static AttributeModifier read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string id;
    std::string name;
    float amount;
    std::int32_t operation;
    std::int32_t operand;
    bool serializable;  //???
};

}  // namespace bedrock_protocol::types::entity
