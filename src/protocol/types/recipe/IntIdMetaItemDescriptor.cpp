/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/IntIdMetaItemDescriptor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/IntIdMetaItemDescriptor.h"

#include <stdexcept>

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::recipe {

using encoding::LE;

IntIdMetaItemDescriptor::IntIdMetaItemDescriptor(std::int16_t id, std::int16_t meta) : id(id), meta(meta)
{
    if (id == 0 && meta != 0) {
        throw std::invalid_argument("Meta cannot be non-zero for air");
    }
}

IntIdMetaItemDescriptor IntIdMetaItemDescriptor::read(encoding::ByteBufferReader &in)
{
    const auto id = LE::readSignedShort(in);
    std::int16_t meta;
    if (id != 0) {
        meta = LE::readSignedShort(in);
    }
    else {
        meta = 0;
    }

    return IntIdMetaItemDescriptor(id, meta);
}

void IntIdMetaItemDescriptor::write(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedShort(out, id);
    if (id != 0) {
        LE::writeSignedShort(out, meta);
    }
}

}  // namespace bedrock_protocol::types::recipe
