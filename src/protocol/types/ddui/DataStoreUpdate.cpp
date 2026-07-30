/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ddui/DataStoreUpdate.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ddui/DataStoreUpdate.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/ddui/update/BoolDataStoreUpdateValue.h"
#include "bedrock_protocol/protocol/types/ddui/update/DataStoreUpdateValueType.h"
#include "bedrock_protocol/protocol/types/ddui/update/DoubleDataStoreUpdateValue.h"
#include "bedrock_protocol/protocol/types/ddui/update/StringDataStoreUpdateValue.h"

namespace bedrock_protocol::types::ddui {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

namespace {
std::unique_ptr<update::DataStoreUpdateValue> readData(encoding::ByteBufferReader &in)
{
    switch (VarInt::readUnsignedInt(in)) {
    case update::DataStoreUpdateValueType::DOUBLE:
        return std::make_unique<update::DoubleDataStoreUpdateValue>(update::DoubleDataStoreUpdateValue::read(in));
    case update::DataStoreUpdateValueType::BOOL:
        return std::make_unique<update::BoolDataStoreUpdateValue>(update::BoolDataStoreUpdateValue::read(in));
    case update::DataStoreUpdateValueType::STRING:
        return std::make_unique<update::StringDataStoreUpdateValue>(update::StringDataStoreUpdateValue::read(in));
    default:
        throw PacketDecodeException("Unknown DataStoreValueType");
    }
}
}  // namespace

DataStoreUpdate DataStoreUpdate::read(encoding::ByteBufferReader &in)
{
    auto name = std::string(CommonTypes::getString(in));
    auto property = std::string(CommonTypes::getString(in));
    auto path = std::string(CommonTypes::getString(in));

    auto data = readData(in);

    const auto updateCount = LE::readUnsignedInt(in);
    const auto pathUpdateCount = LE::readUnsignedInt(in);

    return DataStoreUpdate(
        std::move(name),
        std::move(property),
        std::move(path),
        std::move(data),
        updateCount,
        pathUpdateCount);
}

void DataStoreUpdate::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    CommonTypes::putString(out, property);
    CommonTypes::putString(out, path);
    VarInt::writeUnsignedInt(out, data->getTypeId());
    data->write(out);
    LE::writeUnsignedInt(out, updateCount);
    LE::writeUnsignedInt(out, pathUpdateCount);
}

std::unique_ptr<DataStoreOperation> DataStoreUpdate::clone() const
{
    return std::make_unique<DataStoreUpdate>(name, property, path, data->clone(), updateCount, pathUpdateCount);
}

}  // namespace bedrock_protocol::types::ddui
