/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/DoubleTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/DoubleTag.h"

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

std::string DoubleTag::getTypeName() const
{
    return "Double";
}

std::int32_t DoubleTag::getType() const
{
    return NBT::TAG_Double;
}

DoubleTag DoubleTag::read(NbtStreamReader &reader)
{
    return DoubleTag(reader.readDouble());
}

void DoubleTag::write(NbtStreamWriter &writer) const
{
    writer.writeDouble(value);
}

std::unique_ptr<Tag> DoubleTag::clone() const
{
    return std::make_unique<DoubleTag>(*this);
}

bool DoubleTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const DoubleTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string DoubleTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

}  // namespace bedrock_protocol::nbt::tag
