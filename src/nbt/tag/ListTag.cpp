/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ListTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/ListTag.h"

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"
#include "bedrock_protocol/nbt/ReaderTracker.h"

namespace bedrock_protocol::nbt::tag {

ListTag::ListTag(std::vector<std::unique_ptr<Tag>> value, std::int32_t tagType) : tagType(tagType)
{
    for (auto &tag : value) {
        push(std::move(tag));  //ensure types get checked
    }
}

ListTag::ListTag(const ListTag &other) : tagType(other.tagType)
{
    value.reserve(other.value.size());
    for (const auto &tag : other.value) {
        value.push_back(tag->clone());
    }
}

ListTag &ListTag::operator=(const ListTag &other)
{
    if (this != &other) {
        ListTag copy(other);
        *this = std::move(copy);
    }
    return *this;
}

void ListTag::push(std::unique_ptr<Tag> tag)
{
    checkTagType(*tag);
    value.push_back(std::move(tag));
}

std::unique_ptr<Tag> ListTag::pop()
{
    if (value.empty()) {
        throw std::logic_error("List is empty");
    }
    auto retval = std::move(value.back());
    value.pop_back();
    return retval;
}

void ListTag::unshift(std::unique_ptr<Tag> tag)
{
    checkTagType(*tag);
    value.insert(value.begin(), std::move(tag));
}

std::unique_ptr<Tag> ListTag::shift()
{
    if (value.empty()) {
        throw std::logic_error("List is empty");
    }
    auto retval = std::move(value.front());
    value.erase(value.begin());
    return retval;
}

void ListTag::insert(std::size_t offset, std::unique_ptr<Tag> tag)
{
    checkTagType(*tag);
    if (offset > value.size()) {
        throw std::out_of_range("Offset cannot be negative or larger than the list's current size");
    }
    value.insert(value.begin() + static_cast<std::ptrdiff_t>(offset), std::move(tag));
}

void ListTag::remove(std::size_t offset)
{
    if (offset < value.size()) {
        value.erase(value.begin() + static_cast<std::ptrdiff_t>(offset));
    }
}

const Tag &ListTag::get(std::size_t offset) const
{
    if (offset >= value.size()) {
        throw std::out_of_range("No such tag at offset " + std::to_string(offset));
    }
    return *value[offset];
}

const Tag &ListTag::first() const
{
    if (value.empty()) {
        throw std::logic_error("List is empty");
    }
    return *value[0];
}

const Tag &ListTag::last() const
{
    if (value.empty()) {
        throw std::logic_error("List is empty");
    }
    return *value.back();
}

void ListTag::set(std::size_t offset, std::unique_ptr<Tag> tag)
{
    checkTagType(*tag);
    if (offset > value.size()) {  //allow setting the end offset
        throw std::out_of_range("Offset cannot be negative or larger than the list's current size");
    }
    if (offset == value.size()) {
        value.push_back(std::move(tag));
    }
    else {
        value[offset] = std::move(tag);
    }
}

std::string ListTag::getTypeName() const
{
    return "List";
}

std::int32_t ListTag::getType() const
{
    return NBT::TAG_List;
}

void ListTag::setTagType(std::int32_t type)
{
    if (!value.empty()) {
        throw std::logic_error("Cannot change tag type of non-empty ListTag");
    }
    tagType = type;
}

void ListTag::checkTagType(const Tag &tag)
{
    const auto type = tag.getType();
    if (type != tagType) {
        if (value.empty()) {
            tagType = type;
        }
        else {
            throw std::invalid_argument("Invalid tag of type " + std::to_string(type) +
                                        " assigned to ListTag, expected " + std::to_string(tagType));
        }
    }
}

ListTag ListTag::read(NbtStreamReader &reader, ReaderTracker &tracker)
{
    std::vector<std::unique_ptr<Tag>> value;
    const std::int32_t tagType = reader.readByte();
    const std::int32_t size = reader.readInt();

    if (size > 0) {
        if (tagType == NBT::TAG_End) {
            throw NbtDataException("Unexpected non-empty list of TAG_End");
        }

        tracker.protectDepth([size, tagType, &reader, &tracker, &value]() {
            for (std::int32_t i = 0; i < size; ++i) {
                value.push_back(NBT::createTag(tagType, reader, tracker));
            }
        });
    }
    return ListTag(std::move(value), tagType);
}

void ListTag::write(NbtStreamWriter &writer) const
{
    writer.writeByte(static_cast<std::uint8_t>(tagType));
    writer.writeInt(static_cast<std::int32_t>(value.size()));
    for (const auto &tag : value) {
        tag->write(writer);
    }
}

std::string ListTag::stringifyValue(int indentation) const
{
    std::string str = "{\n";
    for (const auto &tag : value) {
        str += std::string(static_cast<std::size_t>(indentation + 1) * 2, ' ') + tag->toString(indentation + 1) + "\n";
    }
    return str + std::string(static_cast<std::size_t>(indentation) * 2, ' ') + "}";
}

std::unique_ptr<Tag> ListTag::clone() const
{
    return std::make_unique<ListTag>(*this);
}

bool ListTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const ListTag *>(&that);
    if (other == nullptr || value.size() != other->value.size()) {
        return false;
    }

    for (std::size_t k = 0; k < value.size(); ++k) {
        if (!value[k]->equals(*other->value[k])) {
            return false;
        }
    }

    return true;
}

}  // namespace bedrock_protocol::nbt::tag
