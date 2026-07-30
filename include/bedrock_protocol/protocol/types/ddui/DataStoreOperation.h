/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ddui/DataStoreOperation.php
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

namespace bedrock_protocol::types::ddui {

/**
 * @see ServerboundDataStorePacket
 * @see ClientboundDataStorePacket
 */
class DataStoreOperation {
public:
    virtual ~DataStoreOperation() = default;

    [[nodiscard]] virtual std::uint32_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<DataStoreOperation> clone() const = 0;

protected:
    DataStoreOperation() = default;
    DataStoreOperation(const DataStoreOperation &) = default;
    DataStoreOperation &operator=(const DataStoreOperation &) = default;
};

}  // namespace bedrock_protocol::types::ddui
