/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/StringTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/StringTag.h"

#include <utility>

#include "bedrock_protocol/nbt/InvalidTagValueException.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

StringTag::StringTag(std::string value)
{
    if (value.size() > 32767) {
        throw InvalidTagValueException("StringTag cannot hold more than 32767 bytes, got string of length " +
                                       std::to_string(value.size()));
    }
    this->value = std::move(value);
}

std::string StringTag::getTypeName() const
{
    return "String";
}

std::int32_t StringTag::getType() const
{
    return NBT::TAG_String;
}

StringTag StringTag::read(NbtStreamReader &reader)
{
    return StringTag(reader.readString());
}

void StringTag::write(NbtStreamWriter &writer) const
{
    writer.writeString(value);
}

std::unique_ptr<Tag> StringTag::clone() const
{
    return std::make_unique<StringTag>(*this);
}

bool StringTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const StringTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string StringTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return "\"" + value + "\"";
}

}  // namespace bedrock_protocol::nbt::tag
