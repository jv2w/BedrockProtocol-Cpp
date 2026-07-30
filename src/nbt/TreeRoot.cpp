/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/TreeRoot.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/TreeRoot.h"

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"

namespace bedrock_protocol::nbt {
namespace {

/** pocketmine\utils\Limits::INT16_MAX */
constexpr std::size_t LIMITS_INT16_MAX = 32767;

}  // namespace

TreeRoot::TreeRoot(std::unique_ptr<tag::Tag> root, std::string name)
{
    if (name.size() > LIMITS_INT16_MAX) {
        throw std::invalid_argument("Tag name must be at most " + std::to_string(LIMITS_INT16_MAX) +
                                    " bytes, but got " + std::to_string(name.size()) + " bytes");
    }
    this->root = std::move(root);
    this->name = std::move(name);
}

TreeRoot::TreeRoot(const TreeRoot &other) : root(other.root->clone()), name(other.name)
{
}

TreeRoot &TreeRoot::operator=(const TreeRoot &other)
{
    if (this != &other) {
        root = other.root->clone();
        name = other.name;
    }
    return *this;
}

const tag::CompoundTag &TreeRoot::mustGetCompoundTag() const
{
    const auto *compound = dynamic_cast<const tag::CompoundTag *>(root.get());
    if (compound != nullptr) {
        return *compound;
    }
    throw NbtDataException("Root is not a TAG_Compound");
}

bool TreeRoot::equals(const TreeRoot &that) const
{
    return name == that.name && root->equals(*that.root);
}

std::string TreeRoot::toString() const
{
    return "ROOT {\n  " + (!name.empty() ? "\"" + name + "\" => " : "") + root->toString(1) + "\n}";
}

}  // namespace bedrock_protocol::nbt
