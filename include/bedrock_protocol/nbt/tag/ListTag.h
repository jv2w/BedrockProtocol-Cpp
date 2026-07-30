/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ListTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt {
class NbtStreamReader;
class NbtStreamWriter;
class ReaderTracker;
}  // namespace bedrock_protocol::nbt

namespace bedrock_protocol::nbt::tag {

class ListTag final : public Tag {
public:
    explicit ListTag(std::vector<std::unique_ptr<Tag>> value = {}, std::int32_t tagType = NBT::TAG_End);

    ListTag(const ListTag &other);
    ListTag &operator=(const ListTag &other);
    ListTag(ListTag &&) = default;
    ListTag &operator=(ListTag &&) = default;
    ~ListTag() override = default;

    [[nodiscard]] const std::vector<std::unique_ptr<Tag>> &getValue() const { return value; }

    [[nodiscard]] std::size_t count() const { return value.size(); }

    [[nodiscard]] std::size_t getCount() const { return value.size(); }

    /**
     * Appends the specified tag to the end of the list.
     */
    void push(std::unique_ptr<Tag> tag);

    /**
     * Removes the last tag from the list and returns it.
     */
    std::unique_ptr<Tag> pop();

    /**
     * Adds the specified tag to the start of the list.
     */
    void unshift(std::unique_ptr<Tag> tag);

    /**
     * Removes the first tag from the list and returns it.
     */
    std::unique_ptr<Tag> shift();

    /**
     * Inserts a tag into the list between existing tags, at the specified offset. Later values in the list are moved up
     * by 1 position.
     *
     * @throws std::out_of_range if the offset is not within the bounds of the list
     */
    void insert(std::size_t offset, std::unique_ptr<Tag> tag);

    /**
     * Removes a value from the list. All later tags in the list are moved down by 1 position.
     */
    void remove(std::size_t offset);

    /**
     * Returns the tag at the specified offset.
     *
     * @throws std::out_of_range if the offset is not within the bounds of the list
     */
    [[nodiscard]] const Tag &get(std::size_t offset) const;

    /**
     * Returns the element in the first position of the list, without removing it.
     */
    [[nodiscard]] const Tag &first() const;

    /**
     * Returns the element in the last position in the list (the end), without removing it.
     */
    [[nodiscard]] const Tag &last() const;

    /**
     * Overwrites the tag at the specified offset.
     *
     * @throws std::out_of_range if the offset is not within the bounds of the list
     */
    void set(std::size_t offset, std::unique_ptr<Tag> tag);

    /**
     * Returns whether a tag exists at the specified offset.
     */
    [[nodiscard]] bool isset(std::size_t offset) const { return offset < value.size(); }

    /**
     * Returns whether there are any tags in the list.
     */
    [[nodiscard]] bool empty() const { return value.empty(); }

    [[nodiscard]] std::int32_t getType() const override;

    /**
     * Returns the type of tag contained in this list.
     */
    [[nodiscard]] std::int32_t getTagType() const { return tagType; }

    /**
     * Sets the type of tag that can be added to this list. If TAG_End is used, the type will be auto-detected from the
     * first tag added to the list.
     *
     * @throws std::logic_error if the list is not empty
     *
     * @deprecated As of 1.1, an empty list's type will always be inferred from the first Tag to be inserted.
     * Therefore, this function is now useless.
     */
    void setTagType(std::int32_t type);

    static ListTag read(NbtStreamReader &reader, ReaderTracker &tracker);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

    [[nodiscard]] auto begin() const { return value.begin(); }

    [[nodiscard]] auto end() const { return value.end(); }

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    /**
     * Type-checks the given Tag for addition to the list, updating the list tag type as appropriate.
     *
     * @throws std::invalid_argument if the tag type is not compatible.
     */
    void checkTagType(const Tag &tag);

    std::int32_t tagType;
    std::vector<std::unique_ptr<Tag>> value;
};

}  // namespace bedrock_protocol::nbt::tag
