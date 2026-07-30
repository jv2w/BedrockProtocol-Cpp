/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/StringIdMetaItemDescriptor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/StringIdMetaItemDescriptor.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::LE;
using serializer::CommonTypes;

StringIdMetaItemDescriptor StringIdMetaItemDescriptor::read(encoding::ByteBufferReader &in)
{
    auto stringId = CommonTypes::getString(in);
    const auto meta = LE::readUnsignedShort(in);

    return StringIdMetaItemDescriptor(std::move(stringId), meta);
}

void StringIdMetaItemDescriptor::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, id);
    LE::writeUnsignedShort(out, meta);
}

}  // namespace bedrock_protocol::types::recipe
