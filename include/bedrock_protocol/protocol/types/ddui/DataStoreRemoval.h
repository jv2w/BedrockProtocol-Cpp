/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ddui/DataStoreRemoval.php
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
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreOperation.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreOperationType.h"

namespace bedrock_protocol::types::ddui {

/**
 * @see ClientboundDataStorePacket
 */
class DataStoreRemoval final : public DataStoreOperation {
public:
    static constexpr std::uint32_t ID = DataStoreOperationType::REMOVAL;

    explicit DataStoreRemoval(std::string name) : name(std::move(name)) {}

    [[nodiscard]] std::uint32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getName() const { return name; }

    static DataStoreRemoval read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<DataStoreOperation> clone() const override
    {
        return std::make_unique<DataStoreRemoval>(*this);
    }

private:
    std::string name;
};

}  // namespace bedrock_protocol::types::ddui
