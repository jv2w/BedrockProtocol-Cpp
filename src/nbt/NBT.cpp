/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/NBT.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/NBT.h"

#include <string>

#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/nbt/tag/ByteArrayTag.h"
#include "bedrock_protocol/nbt/tag/ByteTag.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/nbt/tag/DoubleTag.h"
#include "bedrock_protocol/nbt/tag/FloatTag.h"
#include "bedrock_protocol/nbt/tag/IntArrayTag.h"
#include "bedrock_protocol/nbt/tag/IntTag.h"
#include "bedrock_protocol/nbt/tag/ListTag.h"
#include "bedrock_protocol/nbt/tag/LongTag.h"
#include "bedrock_protocol/nbt/tag/ShortTag.h"
#include "bedrock_protocol/nbt/tag/StringTag.h"
#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt {

std::unique_ptr<tag::Tag> NBT::createTag(std::int32_t type, NbtStreamReader &reader, ReaderTracker &tracker)
{
    switch (type) {
    case TAG_Byte:
        return std::make_unique<tag::ByteTag>(tag::ByteTag::read(reader));
    case TAG_Short:
        return std::make_unique<tag::ShortTag>(tag::ShortTag::read(reader));
    case TAG_Int:
        return std::make_unique<tag::IntTag>(tag::IntTag::read(reader));
    case TAG_Long:
        return std::make_unique<tag::LongTag>(tag::LongTag::read(reader));
    case TAG_Float:
        return std::make_unique<tag::FloatTag>(tag::FloatTag::read(reader));
    case TAG_Double:
        return std::make_unique<tag::DoubleTag>(tag::DoubleTag::read(reader));
    case TAG_ByteArray:
        return std::make_unique<tag::ByteArrayTag>(tag::ByteArrayTag::read(reader));
    case TAG_String:
        return std::make_unique<tag::StringTag>(tag::StringTag::read(reader));
    case TAG_List:
        return std::make_unique<tag::ListTag>(tag::ListTag::read(reader, tracker));
    case TAG_Compound:
        return std::make_unique<tag::CompoundTag>(tag::CompoundTag::read(reader, tracker));
    case TAG_IntArray:
        return std::make_unique<tag::IntArrayTag>(tag::IntArrayTag::read(reader));
    default:
        throw NbtDataException("Unknown NBT tag type " + std::to_string(type));
    }
}

}  // namespace bedrock_protocol::nbt
