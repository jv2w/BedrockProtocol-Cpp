/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/BaseNbtSerializer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/BaseNbtSerializer.h"

#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/DataDecodeException.h"
#include "bedrock_protocol/nbt/InvalidTagValueException.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/nbt/ReaderTracker.h"

namespace bedrock_protocol::nbt {

TreeRoot BaseNbtSerializer::readRoot(int maxDepth)
{
    const auto type = static_cast<std::int32_t>(readByte());
    if (type == NBT::TAG_End) {
        throw NbtDataException("Found TAG_End at the start of buffer");
    }

    const std::string rootName = readString();
    ReaderTracker tracker(maxDepth);
    return TreeRoot(NBT::createTag(type, *this, tracker), rootName);
}

TreeRoot BaseNbtSerializer::read(encoding::ByteBufferReader &in, int maxDepth)
{
    in_ = &in;

    try {
        return readRoot(maxDepth);
    }
    catch (const encoding::DataDecodeException &e) {
        throw NbtDataException(e.what());
    }
}

std::unique_ptr<tag::Tag> BaseNbtSerializer::readHeadless(encoding::ByteBufferReader &in, std::int32_t rootType,
                                                          int maxDepth)
{
    in_ = &in;

    ReaderTracker tracker(maxDepth);
    try {
        return NBT::createTag(rootType, *this, tracker);
    }
    catch (const encoding::DataDecodeException &e) {
        throw NbtDataException(e.what());
    }
}

std::vector<TreeRoot> BaseNbtSerializer::readMultiple(encoding::ByteBufferReader &in, int maxDepth)
{
    in_ = &in;

    std::vector<TreeRoot> retval;

    while (in.getUnreadLength() > 0) {
        try {
            retval.push_back(readRoot(maxDepth));
        }
        catch (const encoding::DataDecodeException &e) {
            throw NbtDataException(e.what());
        }
    }

    return retval;
}

void BaseNbtSerializer::writeRoot(const TreeRoot &root)
{
    writeByte(static_cast<std::uint8_t>(root.getTag().getType()));
    writeString(root.getName());
    root.getTag().write(*this);
}

void BaseNbtSerializer::write(encoding::ByteBufferWriter &out, const TreeRoot &data)
{
    out_ = &out;

    writeRoot(data);
}

std::string BaseNbtSerializer::write(const TreeRoot &data)
{
    encoding::ByteBufferWriter out;
    write(out, data);
    return out.take();
}

void BaseNbtSerializer::writeHeadless(encoding::ByteBufferWriter &out, const tag::Tag &data)
{
    out_ = &out;
    data.write(*this);
}

std::string BaseNbtSerializer::writeHeadless(const tag::Tag &data)
{
    encoding::ByteBufferWriter out;
    writeHeadless(out, data);
    return out.take();
}

void BaseNbtSerializer::writeMultiple(encoding::ByteBufferWriter &out, const std::vector<TreeRoot> &data)
{
    out_ = &out;
    for (const auto &root : data) {
        writeRoot(root);
    }
}

std::string BaseNbtSerializer::writeMultiple(const std::vector<TreeRoot> &data)
{
    encoding::ByteBufferWriter out;
    writeMultiple(out, data);
    return out.take();
}

std::uint8_t BaseNbtSerializer::readByte()
{
    return encoding::Byte::readUnsigned(*in_);
}

std::int8_t BaseNbtSerializer::readSignedByte()
{
    return encoding::Byte::readSigned(*in_);
}

void BaseNbtSerializer::writeByte(std::uint8_t v)
{
    encoding::Byte::writeUnsigned(*out_, v);
}

std::string BaseNbtSerializer::readByteArray()
{
    const auto length = readInt();
    if (length < 0) {
        throw NbtDataException("Array length cannot be less than zero (" + std::to_string(length) + " < 0)");
    }
    return std::string(in_->readByteArray(static_cast<std::size_t>(length)));
}

void BaseNbtSerializer::writeByteArray(std::string_view v)
{
    writeInt(static_cast<std::int32_t>(v.size()));  //TODO: overflow
    out_->writeByteArray(v);
}

std::size_t BaseNbtSerializer::checkReadStringLength(std::int64_t len)
{
    if (len > 32767) {
        throw NbtDataException("NBT string length too large (" + std::to_string(len) + " > 32767)");
    }
    return static_cast<std::size_t>(len);
}

std::size_t BaseNbtSerializer::checkWriteStringLength(std::size_t len)
{
    if (len > 32767) {
        throw InvalidTagValueException("NBT string length too large (" + std::to_string(len) + " > 32767)");
    }
    return len;
}

std::string BaseNbtSerializer::readString()
{
    return std::string(in_->readByteArray(checkReadStringLength(readShort())));
}

void BaseNbtSerializer::writeString(std::string_view v)
{
    writeShort(static_cast<std::uint16_t>(checkWriteStringLength(v.size())));
    out_->writeByteArray(v);
}

}  // namespace bedrock_protocol::nbt
