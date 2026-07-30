/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/CompoundTag.php
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
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt {
class NbtStreamReader;
class NbtStreamWriter;
class ReaderTracker;
}  // namespace bedrock_protocol::nbt

namespace bedrock_protocol::nbt::tag {

class ListTag;

/**
 * Deviation from PHP: PHP arrays are ordered hash maps, so a plain `array` preserved both insertion
 * order (which is observable on the wire) and O(1) name lookup. Here the children are stored in an
 * insertion-ordered vector of (name, tag) pairs, with a side index for lookup by name.
 */
class CompoundTag final : public Tag {
public:
    using Entry = std::pair<std::string, std::unique_ptr<Tag>>;

    CompoundTag() = default;

    CompoundTag(const CompoundTag &other);
    CompoundTag &operator=(const CompoundTag &other);
    CompoundTag(CompoundTag &&) = default;
    CompoundTag &operator=(CompoundTag &&) = default;
    ~CompoundTag() override = default;

    /**
     * Helper method for easier fluent usage.
     */
    static CompoundTag create() { return CompoundTag(); }

    [[nodiscard]] std::size_t count() const { return value.size(); }

    [[nodiscard]] std::size_t getCount() const { return value.size(); }

    [[nodiscard]] const std::vector<Entry> &getValue() const { return value; }

    /*
     * Here follows many functions of misery for the sake of type safety. We really needs generics in PHP :(
     */

    /**
     * Returns the tag with the specified name, or null if it does not exist.
     */
    [[nodiscard]] const Tag *getTag(const std::string &name) const;

    /**
     * Returns the ListTag with the specified name, or null if it does not exist. Triggers an exception if a tag exists
     * with that name and the tag is not a ListTag.
     *
     * @throws UnexpectedTagTypeException
     */
    [[nodiscard]] const ListTag *getListTag(const std::string &name) const;

    /**
     * Returns the CompoundTag with the specified name, or null if it does not exist. Triggers an exception if a tag
     * exists with that name and the tag is not a CompoundTag.
     */
    [[nodiscard]] const CompoundTag *getCompoundTag(const std::string &name) const;

    /**
     * Sets the specified Tag as a child tag of the CompoundTag at the offset specified by the tag's name.
     */
    CompoundTag &setTag(const std::string &name, std::unique_ptr<Tag> tag);

    /**
     * Removes the child tags with the specified names from the CompoundTag.
     */
    void removeTag(const std::string &name);

    /** @see CompoundTag::removeTag() - variadic form of the PHP original. */
    void removeTag(std::initializer_list<std::string> names);

    /*
     * The following methods are wrappers around getTagValue() with type safety.
     */

    [[nodiscard]] std::int8_t getByte(const std::string &name) const;
    [[nodiscard]] std::int8_t getByte(const std::string &name, std::int8_t defaultValue) const;

    [[nodiscard]] std::int16_t getShort(const std::string &name) const;
    [[nodiscard]] std::int16_t getShort(const std::string &name, std::int16_t defaultValue) const;

    [[nodiscard]] std::int32_t getInt(const std::string &name) const;
    [[nodiscard]] std::int32_t getInt(const std::string &name, std::int32_t defaultValue) const;

    [[nodiscard]] std::int64_t getLong(const std::string &name) const;
    [[nodiscard]] std::int64_t getLong(const std::string &name, std::int64_t defaultValue) const;

    [[nodiscard]] float getFloat(const std::string &name) const;
    [[nodiscard]] float getFloat(const std::string &name, float defaultValue) const;

    [[nodiscard]] double getDouble(const std::string &name) const;
    [[nodiscard]] double getDouble(const std::string &name, double defaultValue) const;

    [[nodiscard]] std::string getByteArray(const std::string &name) const;
    [[nodiscard]] std::string getByteArray(const std::string &name, const std::string &defaultValue) const;

    [[nodiscard]] std::string getString(const std::string &name) const;
    [[nodiscard]] std::string getString(const std::string &name, const std::string &defaultValue) const;

    [[nodiscard]] std::vector<std::int32_t> getIntArray(const std::string &name) const;
    [[nodiscard]] std::vector<std::int32_t> getIntArray(const std::string &name,
                                                        const std::vector<std::int32_t> &defaultValue) const;

    /*
     * The following methods are wrappers around setTag() which create appropriate tag objects on the fly.
     */

    CompoundTag &setByte(const std::string &name, std::int32_t value);

    CompoundTag &setShort(const std::string &name, std::int32_t value);

    CompoundTag &setInt(const std::string &name, std::int32_t value);

    CompoundTag &setLong(const std::string &name, std::int64_t value);

    CompoundTag &setFloat(const std::string &name, float value);

    CompoundTag &setDouble(const std::string &name, double value);

    CompoundTag &setByteArray(const std::string &name, std::string value);

    CompoundTag &setString(const std::string &name, std::string value);

    CompoundTag &setIntArray(const std::string &name, std::vector<std::int32_t> value);

    [[nodiscard]] std::int32_t getType() const override;

    static CompoundTag read(NbtStreamReader &reader, ReaderTracker &tracker);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

    /**
     * Returns a copy of this CompoundTag with values from the given CompoundTag merged into it. Tags that exist both in
     * this tag and the other will be overwritten by the tag in the other.
     *
     * This deep-clones all tags.
     */
    [[nodiscard]] CompoundTag merge(const CompoundTag &other) const;

    [[nodiscard]] auto begin() const { return value.begin(); }

    [[nodiscard]] auto end() const { return value.end(); }

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    /**
     * Returns the child tag with the specified name if it is of type T, throws if a tag of another
     * type exists under that name, and returns nullptr if no such tag exists.
     *
     * @throws UnexpectedTagTypeException
     */
    template <typename T>
    [[nodiscard]] const T *getTypedTag(const std::string &name) const;

    void rebuildIndex();

    std::vector<Entry> value;
    std::unordered_map<std::string, std::size_t> index;
};

}  // namespace bedrock_protocol::nbt::tag
