/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ByteTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/ByteTag.h"

#include "bedrock_protocol/nbt/InvalidTagValueException.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {

ByteTag::ByteTag(std::int32_t value)
{
    if (value < min() || value > max()) {
        throw InvalidTagValueException("Value " + std::to_string(value) + " is outside the allowed range " +
                                       std::to_string(min()) + " - " + std::to_string(max()));
    }
    this->value = static_cast<std::int8_t>(value);
}

std::string ByteTag::getTypeName() const
{
    return "Byte";
}

std::int32_t ByteTag::getType() const
{
    return NBT::TAG_Byte;
}

ByteTag ByteTag::read(NbtStreamReader &reader)
{
    return ByteTag(reader.readSignedByte());
}

void ByteTag::write(NbtStreamWriter &writer) const
{
    writer.writeByte(static_cast<std::uint8_t>(value));
}

std::unique_ptr<Tag> ByteTag::clone() const
{
    return std::make_unique<ByteTag>(*this);
}

bool ByteTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const ByteTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string ByteTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return std::to_string(value);
}

}  // namespace bedrock_protocol::nbt::tag
