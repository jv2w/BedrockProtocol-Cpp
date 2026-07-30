/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueDouble.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValue.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueType.h"

namespace bedrock_protocol::types::cereal {

class DynamicValueDouble final : public DynamicValue {
public:
    static constexpr std::uint32_t ID = DynamicValueType::DOUBLE;

    explicit DynamicValueDouble(double value) : value(value) {}

    [[nodiscard]] std::uint32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::unique_ptr<DynamicValue> clone() const override
    {
        return std::make_unique<DynamicValueDouble>(*this);
    }

    static DynamicValueDouble readValue(encoding::ByteBufferReader &in);

protected:
    void writeValue(encoding::ByteBufferWriter &out) const override;

private:
    double value;
};

}  // namespace bedrock_protocol::types::cereal
