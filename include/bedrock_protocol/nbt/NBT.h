/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/NBT.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

/**
 * Named Binary Tag handling classes
 */
#pragma once

#include <cstdint>
#include <memory>

namespace bedrock_protocol::nbt {

class NbtStreamReader;
class ReaderTracker;

namespace tag {
class Tag;
}

struct NBT final {
    NBT() = delete;

    static constexpr std::int32_t TAG_End = 0;
    static constexpr std::int32_t TAG_Byte = 1;
    static constexpr std::int32_t TAG_Short = 2;
    static constexpr std::int32_t TAG_Int = 3;
    static constexpr std::int32_t TAG_Long = 4;
    static constexpr std::int32_t TAG_Float = 5;
    static constexpr std::int32_t TAG_Double = 6;
    static constexpr std::int32_t TAG_ByteArray = 7;
    static constexpr std::int32_t TAG_String = 8;
    static constexpr std::int32_t TAG_List = 9;
    static constexpr std::int32_t TAG_Compound = 10;
    static constexpr std::int32_t TAG_IntArray = 11;

    /**
     * Maximum nesting depth accepted by the Bedrock network protocol's NBT decoder. Not part of the
     * PHP nbt library itself - PMMP passes this value from PacketSerializer when decoding NBT
     * received over the network. Kept here so that all callers share one constant.
     */
    static constexpr int MAX_DEPTH = 512;

    /**
     * @throws NbtDataException
     */
    static std::unique_ptr<tag::Tag> createTag(std::int32_t type, NbtStreamReader &reader, ReaderTracker &tracker);
};

}  // namespace bedrock_protocol::nbt
