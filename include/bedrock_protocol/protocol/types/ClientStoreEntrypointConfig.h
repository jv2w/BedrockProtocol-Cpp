/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ClientStoreEntrypointConfig.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

/**
 * @see ServerStoreInfoPacket
 */
class ClientStoreEntrypointConfig final {
public:
    ClientStoreEntrypointConfig(std::string storeId, std::string storeName)
        : storeId(std::move(storeId)), storeName(std::move(storeName))
    {
    }

    [[nodiscard]] const std::string &getStoreId() const { return storeId; }

    [[nodiscard]] const std::string &getStoreName() const { return storeName; }

    static ClientStoreEntrypointConfig read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string storeId;
    std::string storeName;
};

}  // namespace bedrock_protocol::types
