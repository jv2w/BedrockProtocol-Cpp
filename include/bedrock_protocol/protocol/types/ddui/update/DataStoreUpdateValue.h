/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ddui/update/DataStoreUpdateValue.php
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

#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::ddui::update {

class DataStoreUpdateValue {
public:
    virtual ~DataStoreUpdateValue() = default;

    [[nodiscard]] virtual std::uint32_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<DataStoreUpdateValue> clone() const = 0;

protected:
    DataStoreUpdateValue() = default;
    DataStoreUpdateValue(const DataStoreUpdateValue &) = default;
    DataStoreUpdateValue &operator=(const DataStoreUpdateValue &) = default;
};

}  // namespace bedrock_protocol::types::ddui::update
