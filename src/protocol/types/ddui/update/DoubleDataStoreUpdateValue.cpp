/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ddui/update/DoubleDataStoreUpdateValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ddui/update/DoubleDataStoreUpdateValue.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::ddui::update {

using encoding::LE;

void DoubleDataStoreUpdateValue::write(encoding::ByteBufferWriter &out) const
{
    LE::writeDouble(out, value);
}

DoubleDataStoreUpdateValue DoubleDataStoreUpdateValue::read(encoding::ByteBufferReader &in)
{
    return DoubleDataStoreUpdateValue(LE::readDouble(in));
}

}  // namespace bedrock_protocol::types::ddui::update
