/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/cereal/DynamicValue.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueBool.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueDouble.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueList.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueLong.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueMap.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueString.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueType.h"

namespace bedrock_protocol::types::cereal {

std::unique_ptr<DynamicValue> DynamicValue::read(encoding::ByteBufferReader &in, std::uint32_t type,
                                                 const int depth)
{
    if (depth > MAX_DEPTH) {
        throw PacketDecodeException("Dynamic value nesting level too deep: reached max depth of " +
                                    std::to_string(MAX_DEPTH));
    }

    //TODO: I don't like putting this here (cyclic dependency) but I don't know where else to put it for now.
    //Really we need to revamp how unions are handled in general, but that's a job for another time
    switch (type) {
    case DynamicValueType::NULL_:
        return nullptr;
    case DynamicValueBool::ID:
        return std::make_unique<DynamicValueBool>(DynamicValueBool::readValue(in));
    case DynamicValueLong::ID:
        return std::make_unique<DynamicValueLong>(DynamicValueLong::readValue(in));
    case DynamicValueDouble::ID:
        return std::make_unique<DynamicValueDouble>(DynamicValueDouble::readValue(in));
    case DynamicValueString::ID:
        return std::make_unique<DynamicValueString>(DynamicValueString::readValue(in));
    case DynamicValueList::ID:
        return std::make_unique<DynamicValueList>(DynamicValueList::readValue(in, depth));
    case DynamicValueMap::ID:
        return std::make_unique<DynamicValueMap>(DynamicValueMap::readValue(in, depth));
    default:
        throw PacketDecodeException("Unknown dynamic value type " + std::to_string(type));
    }
}

}  // namespace bedrock_protocol::types::cereal
