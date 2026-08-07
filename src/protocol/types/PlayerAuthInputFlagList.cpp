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

#include "bedrock_protocol/protocol/types/PlayerAuthInputFlagList.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

void PlayerAuthInputFlagList::checkIndex(const std::int32_t index) const
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Input flag index " + std::to_string(index) + " is out of bounds");
    }
}

bool PlayerAuthInputFlagList::get(const std::int32_t index) const
{
    if (!present) {
        return false;
    }
    checkIndex(index);

    return std::find(ids.begin(), ids.end(), index) != ids.end();
}

void PlayerAuthInputFlagList::set(const std::int32_t index, const bool value)
{
    if (!value) {
        // input_flags.go:55-58 - unsetting a flag on an absent list leaves it absent.
        if (!present) {
            return;
        }
        checkIndex(index);
        const auto at = std::find(ids.begin(), ids.end(), index);
        if (at != ids.end()) {
            ids.erase(at);
        }
        return;
    }

    checkIndex(index);
    present = true;
    if (std::find(ids.begin(), ids.end(), index) == ids.end()) {
        ids.push_back(index);
    }
}

PlayerAuthInputFlagList PlayerAuthInputFlagList::read(encoding::ByteBufferReader &in, const std::int32_t size)
{
    PlayerAuthInputFlagList result;
    result.size = size;
    if (!CommonTypes::getBool(in)) {
        // input_flags.go:80-84 - an absent list ends after its presence bool.
        return result;
    }
    result.present = true;

    // input_flags.go:86-92 - the count is checked before the IDs are read, so a peer cannot claim far
    // more flags than can legally be sent.
    const auto count = VarInt::readUnsignedInt(in);
    if (count > static_cast<std::uint32_t>(size)) {
        throw PacketDecodeException("Too many player auth input flags: " + std::to_string(count));
    }

    result.ids.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto id = VarInt::readSignedInt(in);
        // input_flags.go:96-102 - every ID must be in range and unique.
        if (id < 0 || id >= size) {
            throw PacketDecodeException("Unknown player auth input flag " + std::to_string(id));
        }
        if (std::find(result.ids.begin(), result.ids.end(), id) != result.ids.end()) {
            throw PacketDecodeException("Player auth input flags must be unique, got " + std::to_string(id) +
                                        " twice");
        }
        result.ids.push_back(id);
    }

    return result;
}

void PlayerAuthInputFlagList::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBool(out, present);
    if (!present) {
        return;
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(ids.size()));
    for (const auto id : ids) {
        VarInt::writeSignedInt(out, id);
    }
}

}  // namespace bedrock_protocol::types
