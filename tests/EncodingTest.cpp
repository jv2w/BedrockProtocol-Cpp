/*
 * This file is part of BedrockProtocol for Endstone.
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

}  // namespace

int main()
{
    testVarInt();
    testFixedWidth();
    testFloatingPoint();
    testDecodeErrors();
    testUuid();
    return bedrock_protocol::test::summarise("EncodingTest");
}
