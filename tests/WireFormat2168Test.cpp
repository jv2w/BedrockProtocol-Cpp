/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Asserts the EXACT bytes of the encodings that changed in 1.26.40 (protocol 2168).
 *
 * The deep round-trip suite cannot see these. It compares a packet against itself, so a mistake made
 * consistently in both encode and decode round-trips perfectly: a colour written R,G,B,A instead of
 * B,G,R,A, a fixed int32 that leaked into a varint, or an optional whose presence byte went missing
 * all survive it untouched. The PHP wire-parity gate used to be the answer, but the PHP original is
 * archived at an older protocol and can no longer speak for 2168, so the expectations below are
 * written out by hand from the gophertunnel v1.58.0 reference and cited line by line.
 */

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "TestHarness.h"
#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/ResourcePackClientResponsePacket.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/BoolGameRule.h"
#include "bedrock_protocol/protocol/types/CompressionAlgorithm.h"
#include "bedrock_protocol/protocol/types/IntGameRule.h"
#include "bedrock_protocol/protocol/types/NullGameRule.h"
#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

using namespace bedrock_protocol;
using namespace bedrock_protocol::test;

namespace {

/** Returns true when `haystack` contains `needle` as a byte subsequence. */
bool contains(std::string_view haystack, std::string_view needle)
{
    return haystack.find(needle) != std::string_view::npos;
}

/**
 * A skin carrying exactly one persona tint colour, and nothing else that varies.
 *
 * Everything before the tint list is fixed so that the assertions below can look for byte patterns
 * without having to reproduce the whole skin body.
 */
types::skin::SkinData makeSkinWithTint(std::string pieceType, types::skin::PersonaPieceTintColor::Colors colors)
{
    // SkinImage's constructor demands exactly height * width * 4 bytes.
    types::skin::SkinImage image(1, 1, std::string(4, '\0'));
    std::vector<types::skin::PersonaPieceTintColor> tints;
    tints.emplace_back(std::move(pieceType), colors);

    return types::skin::SkinData("skinId", "playFabId", "resourcePatch", image, /*animations*/ {},
                                 /*capeImage*/ std::nullopt, /*geometryData*/ "",
                                 /*geometryDataEngineVersion*/ "", /*animationData*/ "", /*capeId*/ "",
                                 /*fullSkinId*/ std::nullopt, types::skin::SkinData::ARM_SIZE_WIDE,
                                 color::Color(0x11, 0x22, 0x33, 0x44), /*personaPieces*/ {}, std::move(tints));
}

/**
 * BEARGB is `A | R<<8 | G<<16 | B<<24` written big-endian, so the bytes come out B,G,R,A.
 * Reference: gophertunnel v1.58.0 minecraft/protocol/writer.go:166-169.
 *
 * Checked through the skin codec rather than against the colour helper directly, so that swapping
 * the helper out later cannot quietly change the wire.
 */
void testSkinColourIsBgra()
{
    const auto skin = makeSkinWithTint("persona_eyes", {color::Color(1, 2, 3, 4), color::Color(1, 2, 3, 4),
                                                        color::Color(1, 2, 3, 4), color::Color(1, 2, 3, 4)});

    encoding::ByteBufferWriter out;
    serializer::CommonTypes::putSkin(out, skin);
    const std::string_view bytes(out.getData());

    // The skin colour above is R=0x11 G=0x22 B=0x33 A=0x44, so it must appear as 33 22 11 44.
    CHECK_TRUE(contains(bytes, std::string_view("\x33\x22\x11\x44", 4)));
    // ...and never in ARGB or RGBA order, which are the two ways this is usually got wrong.
    CHECK_TRUE(!contains(bytes, std::string_view("\x44\x11\x22\x33", 4)));
    CHECK_TRUE(!contains(bytes, std::string_view("\x11\x22\x33\x44", 4)));

    // The tint colours are a fixed array of four with NO count prefix (skin.go:256-258), so the
    // single tint above contributes exactly four consecutive BEARGB groups.
    CHECK_TRUE(contains(bytes, std::string_view("\x03\x02\x01\x04\x03\x02\x01\x04\x03\x02\x01\x04\x03\x02\x01\x04", 16)));
}

/**
 * A persona tint's piece type loses its `persona_` prefix on the wire and is restored on read.
 * Reference: gophertunnel v1.58.0 minecraft/protocol/skin.go:263-279.
 */
void testTintPieceTypeIsAbbreviated()
{
    const types::skin::PersonaPieceTintColor::Colors colors{};

    encoding::ByteBufferWriter out;
    serializer::CommonTypes::putSkin(out, makeSkinWithTint("persona_eyes", colors));
    const std::string_view bytes(out.getData());

    // A length-prefixed "eyes", and no trace of the in-memory name.
    CHECK_TRUE(contains(bytes, std::string_view("\x04" "eyes", 5)));
    CHECK_TRUE(!contains(bytes, std::string_view("persona_eyes")));

    encoding::ByteBufferReader in(bytes);
    const auto decoded = serializer::CommonTypes::getSkin(in);
    CHECK_EQ(decoded.getPieceTintColors().size(), std::size_t{1});
    CHECK_EQ(decoded.getPieceTintColors()[0].getPieceType(), std::string("persona_eyes"));

    // `persona_hand` is the one entry that is not a plain prefix strip: it goes out as "hands".
    encoding::ByteBufferWriter handOut;
    serializer::CommonTypes::putSkin(handOut, makeSkinWithTint("persona_hand", colors));
    const std::string_view handBytes(handOut.getData());
    CHECK_TRUE(contains(handBytes, std::string_view("\x05" "hands", 6)));

    encoding::ByteBufferReader handIn(handBytes);
    const auto decodedHand = serializer::CommonTypes::getSkin(handIn);
    CHECK_EQ(decodedHand.getPieceTintColors()[0].getPieceType(), std::string("persona_hand"));
}

/** An absent optional is exactly one 0x00 byte, and a present one is 0x01 followed by the value. */
void testOptionalAbsenceIsOneZeroByte()
{
    encoding::ByteBufferWriter absent;
    serializer::CommonTypes::writeOptional(absent, std::optional<bool>{},
                                           [](encoding::ByteBufferWriter &w, bool v) {
                                               serializer::CommonTypes::putBool(w, v);
                                           });
    CHECK_BYTES(std::string_view(absent.getData()), std::string_view("\x00", 1));

    encoding::ByteBufferWriter present;
    serializer::CommonTypes::writeOptional(present, std::optional<bool>{true},
                                           [](encoding::ByteBufferWriter &w, bool v) {
                                               serializer::CommonTypes::putBool(w, v);
                                           });
    CHECK_BYTES(std::string_view(present.getData()), std::string_view("\x01\x01", 2));
}

/**
 * An int game rule's value is a FIXED 4-byte little-endian uint32 in 1.26.40, in both StartGame and
 * GameRulesChanged. Reference: gophertunnel v1.58.0 minecraft/protocol/writer.go:215-218.
 *
 * Value 1 is the whole point of the check: as a varint it would be a single 0x01 byte.
 */
void testIntGameRuleIsFixedWidth()
{
    serializer::CommonTypes::GameRules rules;
    rules.emplace_back("r", std::make_unique<types::IntGameRule>(1, true));

    encoding::ByteBufferWriter out;
    serializer::CommonTypes::putGameRules(out, rules);

    // varuint32 count 1 | string "r" | bool playerModifiable | varuint32 type 2 | LE uint32 1
    CHECK_BYTES(std::string_view(out.getData()), std::string_view("\x01\x01r\x01\x02\x01\x00\x00\x00", 9));
}

/**
 * Type 0 is a game rule with no payload at all — new in 1.26.40.
 * Reference: gophertunnel v1.58.0 minecraft/protocol/writer.go:208-210, reader.go:272-273.
 */
void testNullGameRuleHasNoPayload()
{
    serializer::CommonTypes::GameRules rules;
    rules.emplace_back("r", std::make_unique<types::NullGameRule>(true));

    encoding::ByteBufferWriter out;
    serializer::CommonTypes::putGameRules(out, rules);
    CHECK_BYTES(std::string_view(out.getData()), std::string_view("\x01\x01r\x01\x00", 5));

    encoding::ByteBufferReader in(out.getData());
    const auto decoded = serializer::CommonTypes::getGameRules(in);
    CHECK_EQ(decoded.size(), std::size_t{1});
    CHECK_EQ(decoded[0].second->getTypeId(), types::NullGameRule::ID);
}

/**
 * ResourcePackClientResponse writes a varuint32 response followed by its name, and only carries the
 * pack list for SendPacks. Reference: gophertunnel v1.58.0
 * minecraft/protocol/packet/resource_pack_client_response.go:30-41.
 *
 * The enum was rebased from `iota + 1` to `iota` in this version, so the numbers below are also
 * guarding against the old off-by-one coming back.
 */
void testResourcePackClientResponseBytes()
{
    {
        auto packet = ResourcePackClientResponsePacket::create(ResourcePackClientResponsePacket::STATUS_COMPLETED, {});
        encoding::ByteBufferWriter out;
        packet.encode(out);
        // Completed == 3, name "resourcepackstackfinished" (25 bytes), and NO pack list follows.
        const std::string_view bytes(out.getData());
        CHECK_TRUE(contains(bytes, std::string_view("\x03\x19" "resourcepackstackfinished", 27)));
    }
    {
        auto packet = ResourcePackClientResponsePacket::create(ResourcePackClientResponsePacket::STATUS_SEND_PACKS,
                                                               {"a"});
        encoding::ByteBufferWriter out;
        packet.encode(out);
        // SendPacks == 1, name "downloading" (11 bytes), then a varuint32-counted list of one string.
        CHECK_TRUE(contains(std::string_view(out.getData()),
                       std::string_view("\x01\x0b" "downloading" "\x01\x01" "a", 16)));
    }
}

/**
 * "No compression" is 0xffff, not 255. Reference: gophertunnel v1.58.0
 * minecraft/protocol/packet/network_settings.go:11.
 */
void testCompressionNoneSentinel()
{
    CHECK_EQ(types::CompressionAlgorithm::NONE, std::uint16_t{0xffff});
}

}  // namespace

int main()
{
    testSkinColourIsBgra();
    testTintPieceTypeIsAbbreviated();
    testOptionalAbsenceIsOneZeroByte();
    testIntGameRuleIsFixedWidth();
    testNullGameRuleHasNoPayload();
    testResourcePackClientResponseBytes();
    testCompressionNoneSentinel();

    return summarise("WireFormat2168Test");
}
