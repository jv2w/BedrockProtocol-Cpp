/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Exercises every registered packet without needing a live server:
 *
 *   1. PacketPool can construct all of them, and each reports the ID it was registered under.
 *   2. A default-constructed packet encodes without crashing.
 *   3. Those bytes decode back, consume the whole buffer, and re-encode byte-identically.
 *
 * Step 3 is the one that matters: an asymmetry between decodePayload and encodePayload is the
 * failure mode that silently corrupts packets on the wire, and it is invisible until something
 * re-encodes a packet the way the bridge does.
 *
 * A packet that legitimately cannot round-trip from its default state (because a field is only
 * meaningful when another field is set) is listed in EXPECTED_ENCODE_FAILURES with a reason.
 */

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#ifdef _MSC_VER
#include <eh.h>
#endif

#include "TestHarness.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/AllPackets.h"
#include "bedrock_protocol/protocol/PacketPool.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"

using namespace bedrock_protocol;

namespace {

/**
 * Packets that cannot be encoded straight from their default-constructed state.
 *
 * These all have a required member that PHP leaves as an uninitialised typed property - reading one
 * before assignment is an Error there too - so "default state does not encode" mirrors the original
 * rather than indicating a wire bug. They are excluded from the round-trip gate; everything else
 * must round-trip byte-identically.
 */
const std::map<std::string_view, std::string_view> EXPECTED_INCOMPLETE = {
    {"InventoryTransactionPacket", "trData is a required polymorphic member"},
    {"AvailableActorIdentifiersPacket", "identifiers is a required CacheableNbt"},
    {"LevelEventGenericPacket", "eventData is a required CacheableNbt"},
    {"ServerboundPackSettingChangePacket", "setting is a required polymorphic member"},
    {"SyncWorldClocksPacket", "payload is a required polymorphic member"},
    {"ClientboundAttributeLayerSyncPacket", "payload is a required polymorphic member"},
    {"InventoryContentPacket", "requires a container name"},
    {"ServerboundDataStorePacket", "requires a payload"},
    {"PlayerLocationPacket", "PLAYER_LOCATION_COORDINATES requires a position, as in PHP"},
    {"PlayerAuthInputPacket", "inputFlags must be a BitSet of FLAG_LENGTH bits"},
    {"ClientMovementPredictionSyncPacket", "flags must be a BitSet of FLAG_LENGTH bits"},
    // These two used to pass here, but only because encodePayload dereferenced a disengaged
    // std::optional with operator*, which is undefined behaviour that happened to write something.
    // They now use .value(), which throws - matching PHP, where reading an uninitialised typed
    // property is an Error. The packets did not get worse; the test got honest.
    {"AddItemActorPacket", "item is a required ItemStackWrapper"},
    {"PlayerSkinPacket", "skin is a required SkinData"},
};

struct Outcome {
    int constructed = 0;
    int encoded = 0;
    int roundTripped = 0;
};

#ifdef _MSC_VER
/**
 * Turns a hardware fault into a catchable exception.
 *
 * A packet whose required polymorphic member is still null dereferences it and takes the process
 * down, which would report one packet per run. Translating the fault lets one run classify all 229.
 */
void translateStructuredException(unsigned int code, struct _EXCEPTION_POINTERS *)
{
    // Spelled out rather than including <windows.h>, whose min/max macros collide with field names
    // in the ported types.
    constexpr unsigned int ACCESS_VIOLATION = 0xC0000005;
    throw std::runtime_error(code == ACCESS_VIOLATION
                                 ? "access violation (a required member is probably still null)"
                                 : "structured exception " + std::to_string(code));
}
#endif

/** Reports what happened to one packet ID, so the summary can distinguish the failure modes. */
Outcome checkPacket(std::uint32_t pid, std::map<std::string, std::vector<std::string>> &failures)
{
    Outcome outcome;

    auto packet = PacketPool::getInstance().getPacketById(pid);
    if (packet == nullptr) {
        return outcome;
    }
    outcome.constructed = 1;

    const std::string name(packet->getName());


    if (packet->pid() != pid) {
        failures["registered under the wrong ID"].push_back(
            name + " (registered " + std::to_string(pid) + ", reports " + std::to_string(packet->pid()) + ")");
        return outcome;
    }

    encoding::ByteBufferWriter first;
    try {
        packet->encodeBody(first);
    }
    catch (const std::exception &e) {
        failures["encode threw"].push_back(name + ": " + e.what());
        return outcome;
    }
    outcome.encoded = 1;

    auto decoded = PacketPool::getInstance().getPacketById(pid);
    encoding::ByteBufferReader in(first.getData());
    try {
        decoded->decodeBody(in);
    }
    catch (const std::exception &e) {
        failures["decode threw on its own output"].push_back(name + ": " + e.what());
        return outcome;
    }

    if (in.getUnreadLength() != 0) {
        failures["decode left trailing bytes"].push_back(
            name + " (" + std::to_string(in.getUnreadLength()) + " of " + std::to_string(first.getData().size()) + ")");
        return outcome;
    }

    encoding::ByteBufferWriter second;
    try {
        decoded->encodeBody(second);
    }
    catch (const std::exception &e) {
        failures["re-encode threw"].push_back(name + ": " + e.what());
        return outcome;
    }

    if (first.getData() != second.getData()) {
        failures["re-encode produced different bytes"].push_back(
            name + " (" + test::toHex(first.getData()) + " vs " + test::toHex(second.getData()) + ")");
        return outcome;
    }

    // encode()/decode() add the packet ID and the split-screen sub-client IDs, which the bridge
    // relies on whenever it injects a packet. The deep suite checks this for the packets it can
    // fill; running it here as well is what covers the six packets that have no fields at all - for
    // those the default state is the only state there is, so this and the body round-trip above are
    // a complete account of them rather than a sample.
    auto header = PacketPool::getInstance().getPacketById(pid);
    try {
        header->senderSubId = 2;
        header->recipientSubId = 3;

        encoding::ByteBufferWriter headerOut;
        header->encode(headerOut);
        const std::string withHeader = headerOut.getData();
        const std::string &body = first.getData();

        if (withHeader.size() < body.size() ||
            withHeader.compare(withHeader.size() - body.size(), body.size(), body) != 0) {
            failures["encode() body differs from encodeBody()"].push_back(name);
            return outcome;
        }

        auto decodedHeader = PacketPool::getInstance().getPacketById(pid);
        encoding::ByteBufferReader headerIn(withHeader);
        decodedHeader->decode(headerIn);

        if (headerIn.getUnreadLength() != 0) {
            failures["decode() left trailing bytes"].push_back(name);
            return outcome;
        }
        if (decodedHeader->senderSubId != 2 || decodedHeader->recipientSubId != 3) {
            failures["sub-client IDs did not survive the header"].push_back(name);
            return outcome;
        }

        encoding::ByteBufferWriter headerAgain;
        decodedHeader->encode(headerAgain);
        if (headerAgain.getData() != withHeader) {
            failures["header path re-encoded differently"].push_back(name);
            return outcome;
        }
    }
    catch (const std::exception &e) {
        failures["header path threw"].push_back(name + ": " + e.what());
        return outcome;
    }

    outcome.roundTripped = 1;
    return outcome;
}

}  // namespace

int main()
{
#ifdef _MSC_VER
    _set_se_translator(translateStructuredException);
#endif

    std::map<std::string, std::vector<std::string>> failures;
    Outcome totals;

    for (std::uint32_t pid = 0; pid < 512; ++pid) {
        const auto outcome = checkPacket(pid, failures);
        totals.constructed += outcome.constructed;
        totals.encoded += outcome.encoded;
        totals.roundTripped += outcome.roundTripped;
    }

    std::printf("Protocol %d (%.*s)\n", ProtocolInfo::CURRENT_PROTOCOL,
                static_cast<int>(ProtocolInfo::MINECRAFT_VERSION_NETWORK.size()),
                ProtocolInfo::MINECRAFT_VERSION_NETWORK.data());
    std::printf("  registered:    %d\n", totals.constructed);
    std::printf("  encoded:       %d\n", totals.encoded);
    std::printf("  round-tripped: %d\n", totals.roundTripped);

    // A packet on the expected-incomplete list may fail; anything else failing is a real defect.
    int unexpected = 0;
    for (const auto &[category, entries] : failures) {
        std::printf("\n%s (%zu):\n", category.c_str(), entries.size());
        for (const auto &entry : entries) {
            const auto packetName = entry.substr(0, entry.find_first_of(" :("));
            const auto expected = EXPECTED_INCOMPLETE.find(packetName);
            const bool known = expected != EXPECTED_INCOMPLETE.end();
            if (!known) {
                ++unexpected;
            }
            std::printf("    %-6s %s\n", known ? "[ok]" : "[NEW]",
                        entry.size() > 150 ? (entry.substr(0, 150) + "...").c_str() : entry.c_str());
        }
    }

    std::printf("\n%d expected-incomplete packets are excluded from the round-trip gate.\n",
                static_cast<int>(EXPECTED_INCOMPLETE.size()));

    CHECK_EQ(totals.constructed, 229);
    CHECK_EQ(unexpected, 0);
    CHECK_EQ(totals.roundTripped, 229 - static_cast<int>(EXPECTED_INCOMPLETE.size()));
    return test::summarise("PacketRoundTripTest");
}
