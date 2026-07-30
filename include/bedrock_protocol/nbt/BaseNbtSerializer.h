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

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/nbt/NbtStreamReader.h"
#include "bedrock_protocol/nbt/NbtStreamWriter.h"
#include "bedrock_protocol/nbt/TreeRoot.h"
#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt {

/**
 * Base Named Binary Tag encoder/decoder
 *
 * Deviation from PHP: the PHP original owns a BinaryStream which is replaced on every read()/write()
 * call. This port does not own a stream; the caller passes in an encoding::ByteBufferReader or
 * encoding::ByteBufferWriter, and the serializer borrows it for the duration of the call. This makes
 * the serializer usable directly against a packet's buffer, which is how the Bedrock protocol embeds
 * NBT, and avoids copying the payload in and out. The borrowed pointer is only valid while one of the
 * public entry points is on the stack; the NbtStreamReader/NbtStreamWriter primitives must not be
 * called outside of one.
 */
class BaseNbtSerializer : public NbtStreamReader, public NbtStreamWriter {
public:
    /**
     * Default nesting limit, matching the value every protocol call site passes explicitly.
     *
     * This is the DEFAULT rather than 0 on purpose. ReaderTracker treats maxDepth 0 as "unlimited",
     * which in PHP is merely unwise but in C++ is a remote kill: NBT deserialisation recurses once per
     * level, so an unlimited reader handed nested TAG_Compound from an untrusted client exhausts the
     * thread stack and takes the process down with no catchable exception. LevelEventGenericPacket
     * omitted the argument and was exactly that bug. Defaulting to a real limit means a call site that
     * forgets the argument is safe rather than fatal; a caller that genuinely wants no limit must now
     * ask for 0 deliberately.
     */
    static constexpr int DEFAULT_MAX_DEPTH = 512;

    /**
     * Decodes NBT from the given reader and returns it.
     *
     * @throws NbtDataException
     */
    TreeRoot read(encoding::ByteBufferReader &in, int maxDepth = DEFAULT_MAX_DEPTH);

    /**
     * Reads a tag without a header from the buffer and returns it. The tag does not have a name, and the type is not
     * specified by the binary data. Only the tag's raw binary value is present. This could be used if the expected root
     * type is always the same.
     *
     * This format is not usually seen in the wild, but it is used in some places in the Minecraft: Bedrock network
     * protocol.
     *
     * @throws NbtDataException
     */
    std::unique_ptr<tag::Tag> readHeadless(encoding::ByteBufferReader &in, std::int32_t rootType,
                                           int maxDepth = DEFAULT_MAX_DEPTH);

    /**
     * Decodes a list of NBT tags into objects and returns them.
     *
     * TODO: This is only necessary because we don't have a streams API worth mentioning. Get rid of this in the future.
     *
     * @throws NbtDataException
     */
    std::vector<TreeRoot> readMultiple(encoding::ByteBufferReader &in, int maxDepth = DEFAULT_MAX_DEPTH);

    void write(encoding::ByteBufferWriter &out, const TreeRoot &data);

    std::string write(const TreeRoot &data);

    /**
     * Writes a nameless tag without any header information. The reader of the data must know what type to expect, as
     * it is not specified in the data.
     *
     * @see BaseNbtSerializer::readHeadless()
     */
    void writeHeadless(encoding::ByteBufferWriter &out, const tag::Tag &data);

    std::string writeHeadless(const tag::Tag &data);

    void writeMultiple(encoding::ByteBufferWriter &out, const std::vector<TreeRoot> &data);

    std::string writeMultiple(const std::vector<TreeRoot> &data);

    std::uint8_t readByte() override;

    std::int8_t readSignedByte() override;

    void writeByte(std::uint8_t v) override;

    std::string readByteArray() override;

    void writeByteArray(std::string_view v) override;

    std::string readString() override;

    /**
     * @throws InvalidTagValueException if the string is too long
     */
    void writeString(std::string_view v) override;

protected:
    /**
     * @throws NbtDataException
     */
    static std::size_t checkReadStringLength(std::int64_t len);

    /**
     * @throws InvalidTagValueException
     */
    static std::size_t checkWriteStringLength(std::size_t len);

    encoding::ByteBufferReader *in_ = nullptr;   // borrowed for the duration of a read* call
    encoding::ByteBufferWriter *out_ = nullptr;  // borrowed for the duration of a write* call

private:
    /**
     * @throws encoding::DataDecodeException
     * @throws NbtDataException
     */
    TreeRoot readRoot(int maxDepth);

    void writeRoot(const TreeRoot &root);
};

}  // namespace bedrock_protocol::nbt

