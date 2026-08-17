/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pmmp/encoding (ByteBufferReader)
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstddef>
#include <string_view>

#include "bedrock_protocol/encoding/DataDecodeException.h"
#include "bedrock_protocol/encoding/ProtocolDialect.h"

namespace bedrock_protocol::encoding {

/**
 * Sequential reader over a fixed byte buffer.
 *
 * The reader is NON-OWNING: the viewed buffer must outlive the reader, and any byte range returned
 * by readByteArray() points into that buffer. Store results as std::string if they need to outlive
 * the read.
 */
class ByteBufferReader {
public:
    explicit ByteBufferReader(std::string_view data, std::size_t offset = 0,
                              ProtocolDialect dialect = CURRENT_DIALECT)
        : data_(data), offset_(offset), dialect_(dialect)
    {
    }

    /** Returns the whole underlying buffer, regardless of the current offset. */
    [[nodiscard]] std::string_view getData() const noexcept { return data_; }

    /** Returns the offset of the next byte to be read. */
    [[nodiscard]] std::size_t getOffset() const noexcept { return offset_; }

    /**
     * Moves the read pointer to the given absolute offset.
     *
     * @throws DataDecodeException if the offset lies beyond the end of the buffer
     */
    void setOffset(std::size_t offset)
    {
        if (offset > data_.size()) {
            throw DataDecodeException("Offset " + std::to_string(offset) + " is beyond the end of the buffer");
        }
        offset_ = offset;
    }

    /** Returns the number of bytes remaining between the read pointer and the end of the buffer. */
    [[nodiscard]] std::size_t getUnreadLength() const noexcept { return data_.size() - offset_; }

    /**
     * Returns the wire layout these bytes are being read as.
     *
     * Only the handful of fields whose layout differs inside one protocol number consult this; every
     * other field reads the same bytes either way. See ProtocolDialect.h.
     */
    [[nodiscard]] ProtocolDialect getDialect() const noexcept { return dialect_; }

    /** Selects the wire layout these bytes are read as. */
    void setDialect(ProtocolDialect dialect) noexcept { dialect_ = dialect; }

    /**
     * Reads the given number of bytes and advances the read pointer past them.
     *
     * The returned view points into the underlying buffer and is only valid for as long as it is.
     *
     * @throws DataDecodeException if fewer than the requested number of bytes remain
     */
    std::string_view readByteArray(std::size_t length)
    {
        if (length > getUnreadLength()) {
            throw DataDecodeException("Need " + std::to_string(length) + " bytes, but only " +
                                      std::to_string(getUnreadLength()) + " are available");
        }
        const auto result = data_.substr(offset_, length);
        offset_ += length;
        return result;
    }

private:
    std::string_view data_;
    std::size_t offset_ = 0;
    ProtocolDialect dialect_ = CURRENT_DIALECT;
};

}  // namespace bedrock_protocol::encoding
