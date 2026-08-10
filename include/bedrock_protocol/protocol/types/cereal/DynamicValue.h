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
     * Deepest chain of nested lists and maps this decoder will follow.
     *
     * Lists and maps recurse into read(), and each level costs a peer only five bytes on the wire
     * (a four-byte type plus a one-byte count), so a modest packet can ask for tens of thousands of
     * frames and overflow the stack - a crash no exception handler can catch. The NBT decoder bounds
     * its own nesting for the same reason; this is the matching bound for the cereal union, set to
     * the same 512 as nbt::NBT::MAX_DEPTH.
     */
    static constexpr int MAX_DEPTH = 512;

    /**
     * @param depth nesting level of this value, counting from zero at the top of a packet
     * @throws PacketDecodeException if the type is unknown or the nesting exceeds MAX_DEPTH
     */
    static std::unique_ptr<DynamicValue> read(encoding::ByteBufferReader &in, std::uint32_t type,
                                              int depth = 0);

protected:
    DynamicValue() = default;
    DynamicValue(const DynamicValue &) = default;
    DynamicValue &operator=(const DynamicValue &) = default;

    virtual void writeValue(encoding::ByteBufferWriter &out) const = 0;
};

}  // namespace bedrock_protocol::types::cereal
