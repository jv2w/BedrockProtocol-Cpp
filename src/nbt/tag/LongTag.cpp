/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/LongTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/LongTag.h"

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

LongTag::LongTag(std::int64_t value) : value(value)
{
}

std::string LongTag::getTypeName() const
{
    return "Long";
}

std::int32_t LongTag::getType() const
{
    return NBT::TAG_Long;
}

LongTag LongTag::read(NbtStreamReader &reader)
{
    return LongTag(reader.readLong());
}

void LongTag::write(NbtStreamWriter &writer) const
{
    writer.writeLong(value);
}

std::unique_ptr<Tag> LongTag::clone() const
{
    return std::make_unique<LongTag>(*this);
}

bool LongTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const LongTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string LongTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

}  // namespace bedrock_protocol::nbt::tag
