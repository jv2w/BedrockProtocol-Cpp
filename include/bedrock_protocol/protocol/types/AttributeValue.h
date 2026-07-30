/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeValue.php
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

namespace bedrock_protocol::types {

/**
 * @see AttributeEnvironment
 */
class AttributeValue {
public:
    virtual ~AttributeValue() = default;

    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<AttributeValue> clone() const = 0;

    /**
     * @throws PacketDecodeException
     */
    static std::unique_ptr<AttributeValue> read(encoding::ByteBufferReader &in);

protected:
    AttributeValue() = default;
    AttributeValue(const AttributeValue &) = default;
    AttributeValue &operator=(const AttributeValue &) = default;
};

}  // namespace bedrock_protocol::types
