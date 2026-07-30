/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ShortTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/ShortTag.h"

#include "bedrock_protocol/nbt/InvalidTagValueException.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

ShortTag::ShortTag(std::int32_t value)
{
    if (value < min() || value > max()) {
        throw InvalidTagValueException("Value " + std::to_string(value) + " is outside the allowed range " +
                                       std::to_string(min()) + " - " + std::to_string(max()));
    }
    this->value = static_cast<std::int16_t>(value);
}

std::string ShortTag::getTypeName() const
{
    return "Short";
}

std::int32_t ShortTag::getType() const
{
    return NBT::TAG_Short;
}

ShortTag ShortTag::read(NbtStreamReader &reader)
{
    return ShortTag(reader.readSignedShort());
}

void ShortTag::write(NbtStreamWriter &writer) const
{
    writer.writeShort(static_cast<std::uint16_t>(value));
}

std::unique_ptr<Tag> ShortTag::clone() const
{
    return std::make_unique<ShortTag>(*this);
}

bool ShortTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const ShortTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string ShortTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

}  // namespace bedrock_protocol::nbt::tag
