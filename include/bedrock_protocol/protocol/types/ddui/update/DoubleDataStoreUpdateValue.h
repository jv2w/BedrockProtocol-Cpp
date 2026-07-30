/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ddui/update/DoubleDataStoreUpdateValue.php
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
#include "bedrock_protocol/protocol/types/ddui/update/DataStoreUpdateValue.h"
#include "bedrock_protocol/protocol/types/ddui/update/DataStoreUpdateValueType.h"

namespace bedrock_protocol::types::ddui::update {

class DoubleDataStoreUpdateValue final : public DataStoreUpdateValue {
public:
    static constexpr std::uint32_t ID = DataStoreUpdateValueType::DOUBLE;

    explicit DoubleDataStoreUpdateValue(double value) : value(value) {}

    [[nodiscard]] double getValue() const { return value; }

    [[nodiscard]] std::uint32_t getTypeId() const override { return ID; }

    void write(encoding::ByteBufferWriter &out) const override;

    static DoubleDataStoreUpdateValue read(encoding::ByteBufferReader &in);

    [[nodiscard]] std::unique_ptr<DataStoreUpdateValue> clone() const override
    {
        return std::make_unique<DoubleDataStoreUpdateValue>(*this);
    }

private:
    const double value;
};

}  // namespace bedrock_protocol::types::ddui::update
