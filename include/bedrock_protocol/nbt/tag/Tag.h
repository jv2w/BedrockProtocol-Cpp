/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/Tag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace bedrock_protocol::nbt {
class NbtStreamWriter;
}

namespace bedrock_protocol::nbt::tag {

/**
 * Base class of all NBT tags.
 *
 * Deviation from PHP: the PHP original carries a `$cloning` flag used for recursive cloning
 * protection, because PHP tags are reference types and may therefore form cycles. In C++ a tag owns
 * its children through std::unique_ptr, so the tag graph is always a tree and a cycle cannot be
 * constructed. safeClone()/makeCopy() are therefore collapsed into a single clone().
 */
class Tag {
public:
    Tag() = default;
    virtual ~Tag() = default;

    Tag(const Tag &) = default;
    Tag &operator=(const Tag &) = default;
    Tag(Tag &&) = default;
    Tag &operator=(Tag &&) = default;

    [[nodiscard]] virtual std::int32_t getType() const = 0;

    virtual void write(NbtStreamWriter &writer) const = 0;

    [[nodiscard]] std::string toString(int indentation = 0) const
    {
        return "TAG_" + getTypeName() + "=" + stringifyValue(indentation);
    }

    /**
     * Deep-copies this tag, including any child tags.
     */
    [[nodiscard]] virtual std::unique_ptr<Tag> clone() const = 0;

    /**
     * Compares this Tag to the given Tag and determines whether or not they are equal, based on type and value.
     * Complex tag types should override this to provide proper value comparison.
     */
    [[nodiscard]] virtual bool equals(const Tag &that) const = 0;

protected:
    [[nodiscard]] virtual std::string getTypeName() const = 0;

    [[nodiscard]] virtual std::string stringifyValue(int indentation) const = 0;
};

inline bool operator==(const Tag &a, const Tag &b)
{
    return a.equals(b);
}

inline bool operator!=(const Tag &a, const Tag &b)
{
    return !a.equals(b);
}

}  // namespace bedrock_protocol::nbt::tag
