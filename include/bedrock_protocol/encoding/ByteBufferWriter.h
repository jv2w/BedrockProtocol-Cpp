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

#include "bedrock_protocol/encoding/ProtocolDialect.h"

namespace bedrock_protocol::encoding {

/** Append-only writer accumulating encoded bytes into an owned buffer. */
class ByteBufferWriter {
public:
    ByteBufferWriter() = default;
    explicit ByteBufferWriter(std::size_t reserve, ProtocolDialect dialect = CURRENT_DIALECT) : dialect_(dialect)
    {
        data_.reserve(reserve);
    }

    /** Returns the bytes written so far. */
    [[nodiscard]] const std::string &getData() const noexcept { return data_; }

    /** Moves the accumulated bytes out of the writer, leaving it empty. */
    [[nodiscard]] std::string take() noexcept { return std::move(data_); }

    /** Appends the given bytes verbatim, without any length prefix. */
    void writeByteArray(std::string_view bytes) { data_.append(bytes); }

    void reserve(std::size_t size) { data_.reserve(size); }

    /** Discards all written bytes, keeping the allocated capacity and the selected dialect. */
    void reset() noexcept { data_.clear(); }

    /**
     * Returns the wire layout these bytes are being written in.
     *
     * Only the handful of fields whose layout differs inside one protocol number consult this; every
     * other field writes the same bytes either way. See ProtocolDialect.h.
     */
    [[nodiscard]] ProtocolDialect getDialect() const noexcept { return dialect_; }

    /** Selects the wire layout these bytes are written in. */
    void setDialect(ProtocolDialect dialect) noexcept { dialect_ = dialect; }

private:
    std::string data_;
    ProtocolDialect dialect_ = CURRENT_DIALECT;
};

}  // namespace bedrock_protocol::encoding
