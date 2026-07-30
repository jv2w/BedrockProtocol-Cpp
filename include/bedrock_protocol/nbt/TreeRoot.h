/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/TreeRoot.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <memory>
#include <string>

#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt {

namespace tag {
class CompoundTag;
}

/**
 * This class wraps around the root Tag for NBT files to avoid losing the name information.
 */
class TreeRoot {
public:
    /**
     * @throws std::invalid_argument if the name is longer than Limits::INT16_MAX bytes
     */
    explicit TreeRoot(std::unique_ptr<tag::Tag> root, std::string name = "");

    TreeRoot(const TreeRoot &other);
    TreeRoot &operator=(const TreeRoot &other);
    TreeRoot(TreeRoot &&) = default;
    TreeRoot &operator=(TreeRoot &&) = default;
    ~TreeRoot() = default;

    [[nodiscard]] const tag::Tag &getTag() const { return *root; }

    /**
     * Helper to reduce boilerplate code for most common NBT usages that use Compound roots.
     *
     * @throws NbtDataException if the root is not a Compound
     */
    [[nodiscard]] const tag::CompoundTag &mustGetCompoundTag() const;

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] bool equals(const TreeRoot &that) const;

    [[nodiscard]] std::string toString() const;

private:
    std::unique_ptr<tag::Tag> root;
    std::string name;
};

}  // namespace bedrock_protocol::nbt
