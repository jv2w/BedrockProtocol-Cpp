/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/ChainedSubCommandRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandRawData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command::raw {

using encoding::VarInt;
using serializer::CommonTypes;

ChainedSubCommandRawData ChainedSubCommandRawData::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);

    std::vector<ChainedSubCommandValueRawData> valueData;
    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        valueData.push_back(ChainedSubCommandValueRawData::read(in));
    }

    return ChainedSubCommandRawData(std::move(name), std::move(valueData));
}

void ChainedSubCommandRawData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(valueData.size()));
    for (const auto &valueDatum : valueData) {
        valueDatum.write(out);
    }
}

}  // namespace bedrock_protocol::types::command::raw
