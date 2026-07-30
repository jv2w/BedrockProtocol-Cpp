/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/IntTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/IntTag.h"

#include "bedrock_protocol/nbt/InvalidTagValueException.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

IntTag::IntTag(std::int64_t value)
{
    if (value < min() || value > max()) {
        throw InvalidTagValueException("Value " + std::to_string(value) + " is outside the allowed range " +
                                       std::to_string(min()) + " - " + std::to_string(max()));
    }
    this->value = static_cast<std::int32_t>(value);
}

std::string IntTag::getTypeName() const
{
    return "Int";
}

std::int32_t IntTag::getType() const
{
    return NBT::TAG_Int;
}

IntTag IntTag::read(NbtStreamReader &reader)
{
    return IntTag(reader.readInt());
}

void IntTag::write(NbtStreamWriter &writer) const
{
    writer.writeInt(value);
}

std::unique_ptr<Tag> IntTag::clone() const
{
    return std::make_unique<IntTag>(*this);
}

bool IntTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const IntTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string IntTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

}  // namespace bedrock_protocol::nbt::tag
