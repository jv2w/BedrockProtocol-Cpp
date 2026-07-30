/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/CacheableNbt.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <string>
#include <utility>

#include "bedrock_protocol/nbt/TreeRoot.h"
#include "bedrock_protocol/nbt/tag/Tag.h"
#include "bedrock_protocol/protocol/serializer/NetworkNbtSerializer.h"

namespace bedrock_protocol::types {

/**
 * @note TTagType is the concrete nbt::tag::Tag subclass held as the root of this NBT tree.
 */
template <typename TTagType>
class CacheableNbt final {
public:
    /** Not present in the PHP original; packets must be default-constructible for PacketPool. */
    CacheableNbt() = default;

    explicit CacheableNbt(TTagType nbtRoot) : nbtRoot(std::move(nbtRoot)) {}

    [[nodiscard]] const TTagType &getRoot() const { return nbtRoot; }

    [[nodiscard]] const std::string &getEncodedNbt() const
    {
        if (!encodedNbt.has_value()) {
            encodedNbt = serializer::NetworkNbtSerializer().write(nbt::TreeRoot(nbtRoot.clone()));
        }
        return *encodedNbt;
    }

private:
    mutable std::optional<std::string> encodedNbt = std::nullopt;

    TTagType nbtRoot;
};

}  // namespace bedrock_protocol::types
