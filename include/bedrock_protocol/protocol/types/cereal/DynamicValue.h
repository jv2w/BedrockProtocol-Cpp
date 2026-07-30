/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValue.php
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

namespace bedrock_protocol::types::cereal {

class DynamicValue {
public:
    virtual ~DynamicValue() = default;

    [[nodiscard]] virtual std::uint32_t getTypeId() const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<DynamicValue> clone() const = 0;

    void write(encoding::ByteBufferWriter &out) const { writeValue(out); }

    /**
     * @throws PacketDecodeException
     */
    static std::unique_ptr<DynamicValue> read(encoding::ByteBufferReader &in, std::uint32_t type);

protected:
    DynamicValue() = default;
    DynamicValue(const DynamicValue &) = default;
    DynamicValue &operator=(const DynamicValue &) = default;

    virtual void writeValue(encoding::ByteBufferWriter &out) const = 0;
};

}  // namespace bedrock_protocol::types::cereal
