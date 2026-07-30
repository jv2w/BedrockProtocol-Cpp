/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/CompoundTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/tag/CompoundTag.h"

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"
#include "bedrock_protocol/nbt/NoSuchTagException.h"
#include "bedrock_protocol/nbt/ReaderTracker.h"
#include "bedrock_protocol/nbt/UnexpectedTagTypeException.h"
#include "bedrock_protocol/nbt/tag/ByteArrayTag.h"
#include "bedrock_protocol/nbt/tag/ByteTag.h"
#include "bedrock_protocol/nbt/tag/DoubleTag.h"
#include "bedrock_protocol/nbt/tag/FloatTag.h"
#include "bedrock_protocol/nbt/tag/IntArrayTag.h"
#include "bedrock_protocol/nbt/tag/IntTag.h"
#include "bedrock_protocol/nbt/tag/ListTag.h"
#include "bedrock_protocol/nbt/tag/LongTag.h"
#include "bedrock_protocol/nbt/tag/ShortTag.h"
#include "bedrock_protocol/nbt/tag/StringTag.h"

namespace bedrock_protocol::nbt::tag {
namespace {

/** pocketmine\utils\Limits::INT16_MAX */
constexpr std::size_t LIMITS_INT16_MAX = 32767;

}  // namespace

CompoundTag::CompoundTag(const CompoundTag &other)
{
    value.reserve(other.value.size());
    for (const auto &[name, tag] : other.value) {
        value.emplace_back(name, tag->clone());
    }
    index = other.index;
}

CompoundTag &CompoundTag::operator=(const CompoundTag &other)
{
    if (this != &other) {
        CompoundTag copy(other);
        *this = std::move(copy);
    }
    return *this;
}

void CompoundTag::rebuildIndex()
{
    index.clear();
    index.reserve(value.size());
    for (std::size_t i = 0; i < value.size(); ++i) {
        index.emplace(value[i].first, i);
    }
}

const Tag *CompoundTag::getTag(const std::string &name) const
{
    const auto it = index.find(name);
    return it != index.end() ? value[it->second].second.get() : nullptr;
}

template <typename T>
const T *CompoundTag::getTypedTag(const std::string &name) const
{
    const Tag *tag = getTag(name);
    if (tag == nullptr) {
        return nullptr;
    }
    const auto *casted = dynamic_cast<const T *>(tag);
    if (casted == nullptr) {
        throw UnexpectedTagTypeException("Tag \"" + name + "\" is of an unexpected type, got " + tag->toString());
    }
    return casted;
}

const ListTag *CompoundTag::getListTag(const std::string &name) const
{
    const Tag *tag = getTag(name);
    if (tag != nullptr) {
        const auto *casted = dynamic_cast<const ListTag *>(tag);
        if (casted == nullptr) {
            throw UnexpectedTagTypeException("Expected a tag of type ListTag, got " + tag->toString());
        }
        return casted;
    }
    return nullptr;
}

const CompoundTag *CompoundTag::getCompoundTag(const std::string &name) const
{
    const Tag *tag = getTag(name);
    if (tag != nullptr) {
        const auto *casted = dynamic_cast<const CompoundTag *>(tag);
        if (casted == nullptr) {
            throw UnexpectedTagTypeException("Expected a tag of type CompoundTag, got " + tag->toString());
        }
        return casted;
    }
    return nullptr;
}

CompoundTag &CompoundTag::setTag(const std::string &name, std::unique_ptr<Tag> tag)
{
    if (name.size() > LIMITS_INT16_MAX) {
        throw std::invalid_argument("Tag name must be at most " + std::to_string(LIMITS_INT16_MAX) +
                                    " bytes, but got " + std::to_string(name.size()) + " bytes");
    }
    const auto it = index.find(name);
    if (it != index.end()) {
        value[it->second].second = std::move(tag);
    }
    else {
        index.emplace(name, value.size());
        value.emplace_back(name, std::move(tag));
    }
    return *this;
}

void CompoundTag::removeTag(const std::string &name)
{
    const auto it = index.find(name);
    if (it != index.end()) {
        value.erase(value.begin() + static_cast<std::ptrdiff_t>(it->second));
        rebuildIndex();
    }
}

void CompoundTag::removeTag(std::initializer_list<std::string> names)
{
    for (const auto &name : names) {
        removeTag(name);
    }
}

/*
 * The following methods are wrappers around getTagValue() with type safety.
 */

std::int8_t CompoundTag::getByte(const std::string &name) const
{
    const auto *tag = getTypedTag<ByteTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::int8_t CompoundTag::getByte(const std::string &name, std::int8_t defaultValue) const
{
    const auto *tag = getTypedTag<ByteTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::int16_t CompoundTag::getShort(const std::string &name) const
{
    const auto *tag = getTypedTag<ShortTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::int16_t CompoundTag::getShort(const std::string &name, std::int16_t defaultValue) const
{
    const auto *tag = getTypedTag<ShortTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::int32_t CompoundTag::getInt(const std::string &name) const
{
    const auto *tag = getTypedTag<IntTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::int32_t CompoundTag::getInt(const std::string &name, std::int32_t defaultValue) const
{
    const auto *tag = getTypedTag<IntTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::int64_t CompoundTag::getLong(const std::string &name) const
{
    const auto *tag = getTypedTag<LongTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::int64_t CompoundTag::getLong(const std::string &name, std::int64_t defaultValue) const
{
    const auto *tag = getTypedTag<LongTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

float CompoundTag::getFloat(const std::string &name) const
{
    const auto *tag = getTypedTag<FloatTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

float CompoundTag::getFloat(const std::string &name, float defaultValue) const
{
    const auto *tag = getTypedTag<FloatTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

double CompoundTag::getDouble(const std::string &name) const
{
    const auto *tag = getTypedTag<DoubleTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

double CompoundTag::getDouble(const std::string &name, double defaultValue) const
{
    const auto *tag = getTypedTag<DoubleTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::string CompoundTag::getByteArray(const std::string &name) const
{
    const auto *tag = getTypedTag<ByteArrayTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::string CompoundTag::getByteArray(const std::string &name, const std::string &defaultValue) const
{
    const auto *tag = getTypedTag<ByteArrayTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::string CompoundTag::getString(const std::string &name) const
{
    const auto *tag = getTypedTag<StringTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::string CompoundTag::getString(const std::string &name, const std::string &defaultValue) const
{
    const auto *tag = getTypedTag<StringTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

std::vector<std::int32_t> CompoundTag::getIntArray(const std::string &name) const
{
    const auto *tag = getTypedTag<IntArrayTag>(name);
    if (tag == nullptr) {
        throw NoSuchTagException("Tag \"" + name + "\" does not exist");
    }
    return tag->getValue();
}

std::vector<std::int32_t> CompoundTag::getIntArray(const std::string &name,
                                                   const std::vector<std::int32_t> &defaultValue) const
{
    const auto *tag = getTypedTag<IntArrayTag>(name);
    return tag != nullptr ? tag->getValue() : defaultValue;
}

/*
 * The following methods are wrappers around setTag() which create appropriate tag objects on the fly.
 */

CompoundTag &CompoundTag::setByte(const std::string &name, std::int32_t value)
{
    return setTag(name, std::make_unique<ByteTag>(value));
}

CompoundTag &CompoundTag::setShort(const std::string &name, std::int32_t value)
{
    return setTag(name, std::make_unique<ShortTag>(value));
}

CompoundTag &CompoundTag::setInt(const std::string &name, std::int32_t value)
{
    return setTag(name, std::make_unique<IntTag>(value));
}

CompoundTag &CompoundTag::setLong(const std::string &name, std::int64_t value)
{
    return setTag(name, std::make_unique<LongTag>(value));
}

CompoundTag &CompoundTag::setFloat(const std::string &name, float value)
{
    return setTag(name, std::make_unique<FloatTag>(value));
}

CompoundTag &CompoundTag::setDouble(const std::string &name, double value)
{
    return setTag(name, std::make_unique<DoubleTag>(value));
}

CompoundTag &CompoundTag::setByteArray(const std::string &name, std::string value)
{
    return setTag(name, std::make_unique<ByteArrayTag>(std::move(value)));
}

CompoundTag &CompoundTag::setString(const std::string &name, std::string value)
{
    return setTag(name, std::make_unique<StringTag>(std::move(value)));
}

CompoundTag &CompoundTag::setIntArray(const std::string &name, std::vector<std::int32_t> value)
{
    return setTag(name, std::make_unique<IntArrayTag>(std::move(value)));
}

std::string CompoundTag::getTypeName() const
{
    return "Compound";
}

std::int32_t CompoundTag::getType() const
{
    return NBT::TAG_Compound;
}

CompoundTag CompoundTag::read(NbtStreamReader &reader, ReaderTracker &tracker)
{
    CompoundTag result;
    tracker.protectDepth([&reader, &tracker, &result]() {
        for (std::int32_t type = reader.readByte(); type != NBT::TAG_End; type = reader.readByte()) {
            const std::string name = reader.readString();
            auto tag = NBT::createTag(type, reader, tracker);
            if (result.getTag(name) != nullptr) {
                //this is technically a corruption case, but it's very common on older PM worlds (pretty much every
                //furnace in PM worlds prior to 2017 is affected), and since we can't extricate this borked data
                //from the rest in Anvil/McRegion worlds, we can't barf on this - it would result in complete loss
                //of the chunk.
                //TODO: add a flag to enable throwing on this (strict mode)
                continue;
            }
            result.setTag(name, std::move(tag));
        }
    });
    return result;
}

void CompoundTag::write(NbtStreamWriter &writer) const
{
    for (const auto &[name, tag] : value) {
        writer.writeByte(static_cast<std::uint8_t>(tag->getType()));
        writer.writeString(name);
        tag->write(writer);
    }
    writer.writeByte(static_cast<std::uint8_t>(NBT::TAG_End));
}

std::string CompoundTag::stringifyValue(int indentation) const
{
    std::string str = "{\n";
    for (const auto &[name, tag] : value) {
        str += std::string(static_cast<std::size_t>(indentation + 1) * 2, ' ') + "\"" + name + "\" => " +
               tag->toString(indentation + 1) + "\n";
    }
    return str + std::string(static_cast<std::size_t>(indentation) * 2, ' ') + "}";
}

std::unique_ptr<Tag> CompoundTag::clone() const
{
    return std::make_unique<CompoundTag>(*this);
}

bool CompoundTag::equals(const Tag &that) const
{
    const auto *other = dynamic_cast<const CompoundTag *>(&that);
    if (other == nullptr || value.size() != other->value.size()) {
        return false;
    }

    for (const auto &[k, v] : value) {
        const Tag *otherTag = other->getTag(k);
        if (otherTag == nullptr || !v->equals(*otherTag)) {
            return false;
        }
    }

    return true;
}

CompoundTag CompoundTag::merge(const CompoundTag &other) const
{
    CompoundTag newTag(*this);

    for (const auto &[k, namedTag] : other) {
        newTag.setTag(k, namedTag->clone());
    }

    return newTag;
}

}  // namespace bedrock_protocol::nbt::tag
