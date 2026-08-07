/*
 * This file is part of BedrockProtocol-Cpp.
 * Not present in the PHP original; models gophertunnel's protocol.InputFlags
 * (minecraft/protocol/input_flags.go:10-104).
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

/**
 * The set of input flags in PlayerAuthInputPacket.
 *
 * As of 1.26.40 this is no longer a packed bitset: it is sent as a presence bool followed by the
 * list of the IDs of the flags that are set, with the IDs kept unique
 * (minecraft/protocol/input_flags.go:78-104).
 *
 * An absent list is not the same as an empty one - it is not sent at all, and reports every flag as
 * unset (minecraft/protocol/input_flags.go:8-9,31-42).
 *
 * @see PlayerAuthInputFlags for the flag ordinals.
 */
class PlayerAuthInputFlagList final {
public:
    /** An absent list: nothing but the presence bool reaches the wire. */
    PlayerAuthInputFlagList() = default;

    /** A present list holding flags in the range [0, size), with none of them set. */
    explicit PlayerAuthInputFlagList(std::int32_t size) : present(true), size(size) {}

    [[nodiscard]] bool isPresent() const { return present; }

    /** The number of flags this list can hold; the wire rejects any ID outside [0, size). */
    [[nodiscard]] std::int32_t getSize() const { return size; }

    /** The IDs of the set flags, in the order they are written. */
    [[nodiscard]] const std::vector<std::int32_t> &getIds() const { return ids; }

    /**
     * @throws std::out_of_range if the index is outside [0, size)
     */
    [[nodiscard]] bool get(std::int32_t index) const;

    /**
     * Setting a flag marks the list as present. Unsetting one on an absent list does nothing.
     *
     * @throws std::out_of_range if the index is outside [0, size)
     */
    void set(std::int32_t index, bool value);

    /**
     * @throws PacketDecodeException
     */
    static PlayerAuthInputFlagList read(encoding::ByteBufferReader &in, std::int32_t size);

    void write(encoding::ByteBufferWriter &out) const;

    [[nodiscard]] bool operator==(const PlayerAuthInputFlagList &that) const
    {
        return present == that.present && size == that.size && ids == that.ids;
    }

    [[nodiscard]] bool operator!=(const PlayerAuthInputFlagList &that) const { return !(*this == that); }

private:
    void checkIndex(std::int32_t index) const;

    bool present = false;
    std::int32_t size = 0;
    std::vector<std::int32_t> ids;
};

}  // namespace bedrock_protocol::types
