/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ddui/update/StringDataStoreUpdateValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ddui/update/StringDataStoreUpdateValue.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::ddui::update {

using serializer::CommonTypes;

void StringDataStoreUpdateValue::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, value);
}

StringDataStoreUpdateValue StringDataStoreUpdateValue::read(encoding::ByteBufferReader &in)
{
    return StringDataStoreUpdateValue(std::string(CommonTypes::getString(in)));
}

}  // namespace bedrock_protocol::types::ddui::update
