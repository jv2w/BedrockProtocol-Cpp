/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/IntArrayTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/IntArrayTag.h"

#include <cstddef>

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

std::string IntArrayTag::getTypeName() const
{
    return "IntArray";
}

std::int32_t IntArrayTag::getType() const
{
    return NBT::TAG_IntArray;
}

IntArrayTag IntArrayTag::read(NbtStreamReader &reader)
{
    return IntArrayTag(reader.readIntArray());
}

void IntArrayTag::write(NbtStreamWriter &writer) const
{
    writer.writeIntArray(value);
}

std::unique_ptr<Tag> IntArrayTag::clone() const
{
    return std::make_unique<IntArrayTag>(*this);
}

bool IntArrayTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const IntArrayTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string IntArrayTag::stringifyValue(int indentation) const
{
    (void)indentation;
    std::string str = "[";
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i != 0) {
            str += ",";
        }
        str += std::to_string(value[i]);
    }
    return str + "]";
}

}  // namespace bedrock_protocol::nbt::tag
