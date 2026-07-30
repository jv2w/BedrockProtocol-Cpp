/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ByteArrayTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/ByteArrayTag.h"

#include <cstddef>

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"

namespace bedrock_protocol::nbt::tag {
namespace {

/** Stand-in for PHP's base64_encode(), used only by the debug stringifier. */
std::string base64_encode(const std::string &input)
{
    static constexpr char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string result;
    result.reserve((input.size() + 2) / 3 * 4);

    std::size_t i = 0;
    for (; i + 2 < input.size(); i += 3) {
        const auto chunk = static_cast<std::uint32_t>(static_cast<unsigned char>(input[i])) << 16 |
                           static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1])) << 8 |
                           static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 2]));
        result += ALPHABET[(chunk >> 18) & 0x3f];
        result += ALPHABET[(chunk >> 12) & 0x3f];
        result += ALPHABET[(chunk >> 6) & 0x3f];
        result += ALPHABET[chunk & 0x3f];
    }

    if (i < input.size()) {
        const bool haveTwo = (i + 1) < input.size();
        const auto chunk = static_cast<std::uint32_t>(static_cast<unsigned char>(input[i])) << 16 |
                           (haveTwo ? static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1])) << 8 : 0);
        result += ALPHABET[(chunk >> 18) & 0x3f];
        result += ALPHABET[(chunk >> 12) & 0x3f];
        result += haveTwo ? ALPHABET[(chunk >> 6) & 0x3f] : '=';
        result += '=';
    }

    return result;
}

}  // namespace

std::string ByteArrayTag::getTypeName() const
{
    return "ByteArray";
}

std::int32_t ByteArrayTag::getType() const
{
    return NBT::TAG_ByteArray;
}

ByteArrayTag ByteArrayTag::read(NbtStreamReader &reader)
{
    return ByteArrayTag(reader.readByteArray());
}

void ByteArrayTag::write(NbtStreamWriter &writer) const
{
    writer.writeByteArray(value);
}

std::unique_ptr<Tag> ByteArrayTag::clone() const
{
    return std::make_unique<ByteArrayTag>(*this);
}

bool ByteArrayTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const ByteArrayTag *>(&that);
    return other != nullptr && value == other->value;
}

std::string ByteArrayTag::stringifyValue(int indentation) const
{
    (void)indentation;
    return "b64:" + base64_encode(value);
}

}  // namespace bedrock_protocol::nbt::tag
