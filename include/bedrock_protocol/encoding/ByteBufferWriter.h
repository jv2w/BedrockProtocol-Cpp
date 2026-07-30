/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pmmp/encoding (ByteBufferWriter)
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

namespace bedrock_protocol::encoding {

/** Append-only writer accumulating encoded bytes into an owned buffer. */
class ByteBufferWriter {
public:
    ByteBufferWriter() = default;
    explicit ByteBufferWriter(std::size_t reserve) { data_.reserve(reserve); }

    /** Returns the bytes written so far. */
    [[nodiscard]] const std::string &getData() const noexcept { return data_; }

    /** Moves the accumulated bytes out of the writer, leaving it empty. */
    [[nodiscard]] std::string take() noexcept { return std::move(data_); }

    /** Appends the given bytes verbatim, without any length prefix. */
    void writeByteArray(std::string_view bytes) { data_.append(bytes); }

    void reserve(std::size_t size) { data_.reserve(size); }

    /** Discards all written bytes, keeping the allocated capacity. */
    void reset() noexcept { data_.clear(); }

private:
    std::string data_;
};

}  // namespace bedrock_protocol::encoding
