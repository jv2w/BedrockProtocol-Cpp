/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Asserts the EXACT bytes of the encodings that changed in 1.26.40 and in 1.26.44 (protocol 2168).
 *
 * The deep round-trip suite cannot see these. It compares a packet against itself, so a mistake made
 * consistently in both encode and decode round-trips perfectly: a colour written R,G,B,A instead of
 * B,G,R,A, a fixed int32 that leaked into a varint, or an optional whose presence byte went missing
 * all survive it untouched. The PHP wire-parity gate used to be the answer, but the PHP original is
 * archived at an older protocol and can no longer speak for 2168, so the expectations below are
 * written out by hand from the gophertunnel v1.58.0 reference and cited line by line.
 *
 * The 1.26.44 group is here for a second reason. That release changed SetScorePacket and left the
 * protocol number at 2168, so the two layouts have to be told apart by a dialect the caller supplies
 * rather than by the number - and no round-trip suite can catch a new presence byte put on the wrong
 * side of the one it wraps, because both sides would agree. Only hand-written bytes can.
 */

#include <array>
#include <cstdint>
#include <exception>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "TestHarness.h"
#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/ProtocolDialect.h"
#include "bedrock_protocol/protocol/ResourcePackClientResponsePacket.h"
#include "bedrock_protocol/protocol/SetScorePacket.h"
#include "bedrock_protocol/protocol/types/ScorePacketEntry.h"
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

// ---- 1.26.44: SetScorePacket's remove entry ----------------------------------------------------

/**
 * The same two-entry remove packet in both of the layouts protocol 2168 covers.
 *
 * Two entries, because the difference only bites when a name is actually PRESENT: an absent name is a
 * single 0x00 under 1.26.40, and 1.26.44 reads that same byte as an absent outer optional, consuming
 * one byte and meaning the same empty name. That accident is why a version-skewed scoreboard corrupts
 * intermittently rather than always, and it is asserted here so a change to either side is visible.
 *
 * References: gophertunnel minecraft/protocol/scoreboard.go, OptionalFunc -> DoubleOptionalFunc in
 * commit 4794743; EndstoneMC/protocol-docs types/RemoveScore.json gaining a leading
 * {"type":"bool","value":true} between the 1.26.43.1 and 1.26.44.3 dumps.
 */
constexpr std::string_view SET_SCORE_REMOVE_V1_26_40(
    "\x02"                                  // entry count: unsigned varint 2
    "\x00" "\x06" "remove" "\x02"           // entry 1: TYPE_REMOVE, its name, scoreboard id 1 (zigzag 2)
    "\x01" "\x03" "obj"                     //          name present, "obj"
    "\x00" "\x06" "remove" "\x04"           // entry 2: TYPE_REMOVE, its name, scoreboard id 2 (zigzag 4)
    "\x00",                                 //          name absent
    25);

constexpr std::string_view SET_SCORE_REMOVE_V1_26_44(
    "\x02"
    "\x00" "\x06" "remove" "\x02"
    "\x01" "\x01" "\x03" "obj"              //          outer present, inner present, "obj"
    "\x00" "\x06" "remove" "\x04"
    "\x01" "\x00",                          //          outer present, inner absent
    27);

/** The packet the two constants above are the encodings of. */
SetScorePacket makeRemovePacket()
{
    types::ScorePacketEntry named;
    named.type = types::ScorePacketEntry::TYPE_REMOVE;
    named.scoreboardId = 1;
    named.objectiveName = "obj";

    types::ScorePacketEntry unnamed;
    unnamed.type = types::ScorePacketEntry::TYPE_REMOVE;
    unnamed.scoreboardId = 2;

    return SetScorePacket::create({named, unnamed});
}

void testSetScoreRemoveBytes()
{
    const auto packet = makeRemovePacket();

    encoding::ByteBufferWriter older(0, encoding::ProtocolDialect::V1_26_40);
    packet.encodeBody(older);
    CHECK_BYTES(std::string_view(older.getData()), SET_SCORE_REMOVE_V1_26_40);

    encoding::ByteBufferWriter newer(0, encoding::ProtocolDialect::V1_26_44);
    packet.encodeBody(newer);
    CHECK_BYTES(std::string_view(newer.getData()), SET_SCORE_REMOVE_V1_26_44);

    // The default has to be the version ProtocolInfo says this library targets, or one of them lies.
    encoding::ByteBufferWriter byDefault;
    packet.encodeBody(byDefault);
    CHECK_BYTES(std::string_view(byDefault.getData()), SET_SCORE_REMOVE_V1_26_44);
}

void testSetScoreRemoveDecodes()
{
    for (const auto &[bytes, dialect] :
         {std::pair{SET_SCORE_REMOVE_V1_26_40, encoding::ProtocolDialect::V1_26_40},
          std::pair{SET_SCORE_REMOVE_V1_26_44, encoding::ProtocolDialect::V1_26_44}}) {
        SetScorePacket packet;
        encoding::ByteBufferReader in(bytes, 0, dialect);
        packet.decodeBody(in);

        CHECK_EQ(in.getUnreadLength(), std::size_t{0});
        CHECK_EQ(packet.entries.size(), std::size_t{2});
        CHECK_EQ(packet.entries[0].objectiveName, std::string("obj"));
        CHECK_EQ(packet.entries[0].scoreboardId, std::int64_t{1});
        CHECK_EQ(packet.entries[1].objectiveName, std::string(""));
        CHECK_EQ(packet.entries[1].scoreboardId, std::int64_t{2});
    }

    // A 1.26.44 remove whose OUTER optional is absent: one byte, with no inner presence byte behind
    // it. Re-encoding writes 01 00 rather than 00 - both mean an empty name, and 01 00 is what the
    // game sends - so this asymmetry is a decision rather than an accident, and is pinned here.
    constexpr std::string_view outerAbsent("\x01" "\x00" "\x06" "remove" "\x02" "\x00", 11);
    SetScorePacket packet;
    encoding::ByteBufferReader in(outerAbsent, 0, encoding::ProtocolDialect::V1_26_44);
    packet.decodeBody(in);
    CHECK_EQ(in.getUnreadLength(), std::size_t{0});
    CHECK_EQ(packet.entries.size(), std::size_t{1});
    CHECK_EQ(packet.entries[0].objectiveName, std::string(""));

    encoding::ByteBufferWriter out(0, encoding::ProtocolDialect::V1_26_44);
    packet.encodeBody(out);
    CHECK_BYTES(std::string_view(out.getData()),
                std::string_view("\x01" "\x00" "\x06" "remove" "\x02" "\x01" "\x00", 12));
}

/** Decoding in one layout and re-encoding in the other is the whole point of carrying a dialect. */
void testSetScoreDialectTranslation()
{
    {
        SetScorePacket packet;
        encoding::ByteBufferReader in(SET_SCORE_REMOVE_V1_26_40, 0, encoding::ProtocolDialect::V1_26_40);
        packet.decodeBody(in);
        encoding::ByteBufferWriter out(0, encoding::ProtocolDialect::V1_26_44);
        packet.encodeBody(out);
        CHECK_BYTES(std::string_view(out.getData()), SET_SCORE_REMOVE_V1_26_44);
    }
    {
        SetScorePacket packet;
        encoding::ByteBufferReader in(SET_SCORE_REMOVE_V1_26_44, 0, encoding::ProtocolDialect::V1_26_44);
        packet.decodeBody(in);
        encoding::ByteBufferWriter out(0, encoding::ProtocolDialect::V1_26_40);
        packet.encodeBody(out);
        CHECK_BYTES(std::string_view(out.getData()), SET_SCORE_REMOVE_V1_26_40);
    }
    // The mismatch this all exists to prevent must not pass silently: read as 1.26.40, the inner
    // presence byte is taken for a string length and the name that follows runs off the end.
    {
        SetScorePacket packet;
        encoding::ByteBufferReader in(SET_SCORE_REMOVE_V1_26_44, 0, encoding::ProtocolDialect::V1_26_40);
        bool threw = false;
        try {
            packet.decodeBody(in);
        }
        catch (const std::exception &) {
            threw = true;
        }
        CHECK_TRUE(threw || in.getUnreadLength() != 0);
    }
}

/**
 * The version string is the only thing that tells the two layouts apart, so what it accepts and what
 * it refuses is part of the wire format.
 */
void testDialectFromGameVersion()
{
    using encoding::ProtocolDialect;
    CHECK_TRUE(encoding::dialectFromGameVersion("1.26.40") == ProtocolDialect::V1_26_40);
    CHECK_TRUE(encoding::dialectFromGameVersion("1.26.43") == ProtocolDialect::V1_26_40);
    // Endstone's Player::getGameVersion() reports four components; the fourth is a hotfix number.
    CHECK_TRUE(encoding::dialectFromGameVersion("1.26.43.2") == ProtocolDialect::V1_26_40);
    CHECK_TRUE(encoding::dialectFromGameVersion("1.26.44") == ProtocolDialect::V1_26_44);
    CHECK_TRUE(encoding::dialectFromGameVersion("1.26.44.3") == ProtocolDialect::V1_26_44);
    CHECK_TRUE(encoding::dialectFromGameVersion("v1.26.44") == ProtocolDialect::V1_26_44);
    // Everything unrecognised is nullopt and never a guess: a wrong guess rewrites live traffic.
    CHECK_TRUE(!encoding::dialectFromGameVersion("").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1.26").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1.26.").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1.26.39").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1.25.44").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1..26.44").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("1.26.44-beta").has_value());
    CHECK_TRUE(!encoding::dialectFromGameVersion("abc").has_value());
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
    testSetScoreRemoveBytes();
    testSetScoreRemoveDecodes();
    testSetScoreDialectTranslation();
    testDialectFromGameVersion();

    return summarise("WireFormat2168Test");
}
