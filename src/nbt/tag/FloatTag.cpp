/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/FloatTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/FloatTag.h"

#include <bit>

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

std::string FloatTag::getTypeName() const
{
    return "Float";
}

std::int32_t FloatTag::getType() const
{
    return NBT::TAG_Float;
}

FloatTag FloatTag::read(NbtStreamReader &reader)
{
    return FloatTag(reader.readFloat());
}

void FloatTag::write(NbtStreamWriter &writer) const
{
    writer.writeFloat(value);
}

std::unique_ptr<Tag> FloatTag::clone() const
{
    return std::make_unique<FloatTag>(*this);
}

std::string FloatTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

bool FloatTag::equals(const Tag &that) const
{
    //the values of TAG_Float are represented in 32 bits (single precision), so we don't want extra precision given
    //by 64-bit in-memory representation to break comparison (e.g. 0.3 != decode(encode(0.3)))
    //this intentionally truncates our value so that it compares as valid
    const auto *other = dynamic_cast<const FloatTag *>(&that);
    return other != nullptr && std::bit_cast<std::uint32_t>(value) == std::bit_cast<std::uint32_t>(other->value);
}

}  // namespace bedrock_protocol::nbt::tag
