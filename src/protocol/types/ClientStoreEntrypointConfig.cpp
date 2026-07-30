/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ClientStoreEntrypointConfig.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ClientStoreEntrypointConfig.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

ClientStoreEntrypointConfig ClientStoreEntrypointConfig::read(encoding::ByteBufferReader &in)
{
    auto storeId = CommonTypes::getString(in);
    auto storeName = CommonTypes::getString(in);

    return ClientStoreEntrypointConfig(std::move(storeId), std::move(storeName));
}

void ClientStoreEntrypointConfig::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, storeId);
    CommonTypes::putString(out, storeName);
}

}  // namespace bedrock_protocol::types
