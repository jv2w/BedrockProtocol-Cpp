/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/WhiskerScopeDataSummary.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/WhiskerScopeDataSummary.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

WhiskerScopeDataSummary WhiskerScopeDataSummary::read(encoding::ByteBufferReader &in)
{
    auto label = CommonTypes::getString(in);
    auto indentation = CommonTypes::getString(in);
    const auto totalHighCostNS = LE::readUnsignedLong(in);
    const auto totalMidCostNS = LE::readUnsignedLong(in);
    const auto totalLowCostNS = LE::readUnsignedLong(in);

    return WhiskerScopeDataSummary(std::move(label), std::move(indentation), totalHighCostNS, totalMidCostNS,
                                   totalLowCostNS);
}

void WhiskerScopeDataSummary::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, label);
    CommonTypes::putString(out, indentation);
    LE::writeUnsignedLong(out, totalHighCostNS);
    LE::writeUnsignedLong(out, totalMidCostNS);
    LE::writeUnsignedLong(out, totalLowCostNS);
}

}  // namespace bedrock_protocol::types
