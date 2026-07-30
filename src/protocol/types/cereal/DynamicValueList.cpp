/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueList.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/cereal/DynamicValueList.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::cereal {

using encoding::LE;
using encoding::VarInt;

std::unique_ptr<DynamicValue> DynamicValueList::clone() const
{
    std::vector<std::unique_ptr<DynamicValue>> copy;
    copy.reserve(values.size());
    for (const auto &value : values) {
        copy.push_back(value != nullptr ? value->clone() : nullptr);
    }
    return std::make_unique<DynamicValueList>(std::move(copy));
}

DynamicValueList DynamicValueList::readValue(encoding::ByteBufferReader &in)
{
    const auto size = VarInt::readUnsignedInt(in);
    std::vector<std::unique_ptr<DynamicValue>> values;
    for (std::uint32_t i = 0; i < size; ++i) {
        const auto type = LE::readUnsignedInt(in);
        values.push_back(DynamicValue::read(in, type));
    }
    return DynamicValueList(std::move(values));
}

void DynamicValueList::writeValue(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(values.size()));
    for (const auto &value : values) {
        LE::writeUnsignedInt(out, value != nullptr ? value->getTypeId() : DynamicValueType::NULL_);
        if (value != nullptr) {
            value->write(out);
        }
    }
}

}  // namespace bedrock_protocol::types::cereal
