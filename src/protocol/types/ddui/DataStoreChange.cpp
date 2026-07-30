/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ddui/DataStoreChange.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ddui/DataStoreChange.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueType.h"

namespace bedrock_protocol::types::ddui {

using encoding::LE;
using serializer::CommonTypes;

DataStoreChange DataStoreChange::read(encoding::ByteBufferReader &in)
{
    auto name = std::string(CommonTypes::getString(in));
    auto property = std::string(CommonTypes::getString(in));
    const auto updateCount = LE::readUnsignedInt(in);

    const auto type = LE::readUnsignedInt(in);
    auto data = cereal::DynamicValue::read(in, type);

    return DataStoreChange(
        std::move(name),
        std::move(property),
        updateCount,
        std::move(data));
}

void DataStoreChange::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    CommonTypes::putString(out, property);
    LE::writeUnsignedInt(out, updateCount);

    //TODO: yucky, we really need to revamp how unions are handled :(
    const auto type = data != nullptr ? data->getTypeId() : cereal::DynamicValueType::NULL_;
    LE::writeUnsignedInt(out, type);
    if (data != nullptr) {
        data->write(out);
    }
}

std::unique_ptr<DataStoreOperation> DataStoreChange::clone() const
{
    return std::make_unique<DataStoreChange>(name, property, updateCount, data != nullptr ? data->clone() : nullptr);
}

}  // namespace bedrock_protocol::types::ddui
