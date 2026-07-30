/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueMap.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/cereal/DynamicValueMap.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::cereal {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

std::unique_ptr<DynamicValue> DynamicValueMap::clone() const
{
    std::vector<std::pair<std::string, std::unique_ptr<DynamicValue>>> copy;
    copy.reserve(value.size());
    for (const auto &entry : value) {
        copy.emplace_back(entry.first, entry.second != nullptr ? entry.second->clone() : nullptr);
    }
    return std::make_unique<DynamicValueMap>(std::move(copy));
}

DynamicValueMap DynamicValueMap::readValue(encoding::ByteBufferReader &in)
{
    std::vector<std::pair<std::string, std::unique_ptr<DynamicValue>>> value;

    for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; i++) {
        auto key = std::string(CommonTypes::getString(in));
        //YIKES! unchecked recursion ?!?!?! thank god this never gets sent by the client...
        const auto type = LE::readUnsignedInt(in);
        CommonTypes::assignKeyed(value, key, DynamicValue::read(in, type));
    }

    return DynamicValueMap(std::move(value));
}

void DynamicValueMap::writeValue(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(value.size()));
    for (const auto &[key, entry] : value) {
        CommonTypes::putString(out, key); //make sure we don't get any unexpected strings casted to int
        LE::writeUnsignedInt(out, entry != nullptr ? entry->getTypeId() : DynamicValueType::NULL_);
        if (entry != nullptr) {
            entry->write(out);
        }
    }
}

}  // namespace bedrock_protocol::types::cereal
