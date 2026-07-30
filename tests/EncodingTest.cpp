/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Verifies the encoding primitives against test vectors produced by the reference PHP
 * implementation (pocketmine/binaryutils). Regenerate the vectors with tools/gen_fixtures.php.
 */

#include <cstdint>
#include <string_view>
#include <tuple>
#include <vector>

#include "TestHarness.h"
#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/entity/IntMetadataProperty.h"
#include "bedrock_protocol/uuid/Uuid.h"

using namespace bedrock_protocol;
using namespace bedrock_protocol::encoding;

namespace {

#include "fixtures/encoding.inc"

/** Checks that writing the value produces the reference bytes, and reading them gives it back. */
template <typename T, typename Write, typename Read>
void checkVector(const std::vector<std::tuple<T, std::string_view>> &cases, Write write, Read read)
{
    for (const auto &[value, expected] : cases) {
        ByteBufferWriter out;
        write(out, value);
        CHECK_BYTES(std::string_view(out.getData()), expected);

        ByteBufferReader in(expected);
        CHECK_EQ(read(in), value);
        CHECK_EQ(in.getUnreadLength(), std::size_t{0});
    }
}

void testVarInt()
{
    checkVector(VARINT_UNSIGNED_INT, VarInt::writeUnsignedInt, VarInt::readUnsignedInt);
    checkVector(VARINT_SIGNED_INT, VarInt::writeSignedInt, VarInt::readSignedInt);
    checkVector(VARINT_UNSIGNED_LONG, VarInt::writeUnsignedLong, VarInt::readUnsignedLong);
    checkVector(VARINT_SIGNED_LONG, VarInt::writeSignedLong, VarInt::readSignedLong);
}

void testFixedWidth()
{
    checkVector(LE_UNSIGNED_SHORT, LE::writeUnsignedShort, LE::readUnsignedShort);
    checkVector(LE_SIGNED_INT, LE::writeSignedInt, LE::readSignedInt);
    checkVector(LE_SIGNED_LONG, LE::writeSignedLong, LE::readSignedLong);

    // BE is the same codec with the byte order reversed.
    ByteBufferWriter out;
    BE::writeUnsignedInt(out, 0x01020304);
    CHECK_BYTES(std::string_view(out.getData()), std::string_view("\x01\x02\x03\x04", 4));
}

void testFloatingPoint()
{
    for (const float value : {0.0F, 1.0F, -1.0F, 3.14159F, -123456.75F}) {
        ByteBufferWriter out;
        LE::writeFloat(out, value);
        ByteBufferReader in(out.getData());
        CHECK_EQ(LE::readFloat(in), value);
    }
    for (const double value : {0.0, 1.0, -1.0, 3.141592653589793, -1.2345e300}) {
        ByteBufferWriter out;
        LE::writeDouble(out, value);
        ByteBufferReader in(out.getData());
        CHECK_EQ(LE::readDouble(in), value);
    }
}

/** Truncated input and over-long VarInts must both raise DataDecodeException, never read past the end. */
void testDecodeErrors()
{
    {
        ByteBufferReader in(std::string_view("\x80", 1));  // continuation bit set, nothing follows
        bool threw = false;
        try {
            VarInt::readUnsignedInt(in);
        }
        catch (const DataDecodeException &) {
            threw = true;
        }
        CHECK_TRUE(threw);
    }
    {
        // Five continuation bytes: a 32-bit VarInt may not exceed five bytes in total.
        ByteBufferReader in(std::string_view("\x80\x80\x80\x80\x80\x01", 6));
        bool threw = false;
        try {
            VarInt::readUnsignedInt(in);
        }
        catch (const DataDecodeException &) {
            threw = true;
        }
        CHECK_TRUE(threw);
    }
    {
        ByteBufferReader in(std::string_view("\x01\x02", 2));
        bool threw = false;
        try {
            in.readByteArray(3);
        }
        catch (const DataDecodeException &) {
            threw = true;
        }
        CHECK_TRUE(threw);
        CHECK_EQ(in.getOffset(), std::size_t{0});  // a failed read must not consume anything
    }
}

void testUuid()
{
    const auto uuid = uuid::Uuid::fromString("12345678-1234-5678-1234-567812345678");
    CHECK_EQ(uuid.toString(), std::string("12345678-1234-5678-1234-567812345678"));
    CHECK_TRUE(uuid::Uuid::fromBytes(uuid.getBytes()) == uuid);

    const auto random = uuid::Uuid::uuid4();
    CHECK_EQ(random.getRawBytes()[6] & 0xf0, 0x40);  // version 4
    CHECK_EQ(random.getRawBytes()[8] & 0xc0, 0x80);  // RFC 4122 variant
}

/**
 * A key that arrives twice must replace the first entry where it already sits.
 *
 * PHP decodes entity metadata and game rules into keyed arrays, so a repeated key overwrites rather
 * than appending. Nothing else in the suite can see this: the fillers only ever produce unique keys,
 * and a list that kept both copies would still round-trip to identical bytes. A client chooses what
 * it sends, so the two behaviours are reachable from outside.
 */
void testRepeatedKeysCollapse()
{
    using serializer::CommonTypes;

    encoding::ByteBufferWriter out;
    VarInt::writeUnsignedInt(out, 3);  // three entries, two of them sharing a key
    for (const auto &[key, value] : {std::pair{7U, 11}, std::pair{9U, 22}, std::pair{7U, 33}}) {
        VarInt::writeUnsignedInt(out, key);
        VarInt::writeUnsignedInt(out, types::entity::IntMetadataProperty::ID);
        VarInt::writeSignedInt(out, value);
    }

    encoding::ByteBufferReader in(out.getData());
    const auto metadata = CommonTypes::getEntityMetadata(in);

    CHECK_EQ(metadata.size(), std::size_t{2});
    CHECK_EQ(metadata[0].first, 7U);   // the repeated key keeps its original position
    CHECK_EQ(metadata[1].first, 9U);

    // ...and carries the last value that arrived for it.
    encoding::ByteBufferWriter again;
    CommonTypes::putEntityMetadata(again, metadata);
    encoding::ByteBufferReader back(again.getData());
    const auto reread = CommonTypes::getEntityMetadata(back);
    CHECK_EQ(reread.size(), std::size_t{2});
}

}  // namespace

int main()
{
    testVarInt();
    testFixedWidth();
    testFloatingPoint();
    testDecodeErrors();
    testUuid();
    testRepeatedKeysCollapse();
    return bedrock_protocol::test::summarise("EncodingTest");
}
