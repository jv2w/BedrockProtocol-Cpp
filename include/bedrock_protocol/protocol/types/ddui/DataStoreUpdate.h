/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ddui/DataStoreUpdate.php
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
#include "bedrock_protocol/protocol/types/ddui/update/DataStoreUpdateValue.h"

namespace bedrock_protocol::types::ddui {

/**
 * @see ServerboundDataStorePacket
 * @see ClientboundDataStorePacket
 */
class DataStoreUpdate final : public DataStoreOperation {
public:
    static constexpr std::uint32_t ID = DataStoreOperationType::UPDATE;

    DataStoreUpdate(std::string name, std::string property, std::string path,
                    std::unique_ptr<update::DataStoreUpdateValue> data, std::uint32_t updateCount,
                    std::uint32_t pathUpdateCount)
        : name(std::move(name)), property(std::move(property)), path(std::move(path)), data(std::move(data)),
          updateCount(updateCount), pathUpdateCount(pathUpdateCount)
    {
    }

    [[nodiscard]] std::uint32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const std::string &getProperty() const { return property; }

    [[nodiscard]] const std::string &getPath() const { return path; }

    [[nodiscard]] const update::DataStoreUpdateValue &getData() const { return *data; }

    [[nodiscard]] std::uint32_t getUpdateCount() const { return updateCount; }

    [[nodiscard]] std::uint32_t getPathUpdateCount() const { return pathUpdateCount; }

    static DataStoreUpdate read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<DataStoreOperation> clone() const override;

private:
    std::string name;
    std::string property;
    std::string path;
    std::unique_ptr<update::DataStoreUpdateValue> data;
    std::uint32_t updateCount;
    std::uint32_t pathUpdateCount;
};

}  // namespace bedrock_protocol::types::ddui
