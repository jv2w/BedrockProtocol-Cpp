/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ddui/DataStoreRemoval.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ddui/DataStoreRemoval.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::ddui {

using serializer::CommonTypes;

DataStoreRemoval DataStoreRemoval::read(encoding::ByteBufferReader &in)
{
    auto name = std::string(CommonTypes::getString(in));

    return DataStoreRemoval(std::move(name));
}

void DataStoreRemoval::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
}

}  // namespace bedrock_protocol::types::ddui
