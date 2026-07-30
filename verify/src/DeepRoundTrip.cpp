/*
 * This file is part of BedrockProtocol-Cpp.
 */

#include "bedrock_protocol/verify/DeepRoundTrip.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/PacketPool.h"

// _set_se_translator needs the Microsoft EH model (/EHa) and the MSVC runtime. clang-cl accepts the
// flag but does not implement the translator, so it is gated on real MSVC and the suite reports
// honestly when it is unavailable rather than pretending the fuzz stages ran.
#if defined(_MSC_VER) && !defined(__clang__)
#define BP_VERIFY_HAS_SE_TRANSLATOR 1
#include <eh.h>
#else
#define BP_VERIFY_HAS_SE_TRANSLATOR 0
#endif

namespace bedrock_protocol::verify {

namespace {

using encoding::ByteBufferReader;
using encoding::ByteBufferWriter;

#if BP_VERIFY_HAS_SE_TRANSLATOR
/**
 * Turns a hardware fault into a catchable exception for the duration of the run.
 *
 * Without this, one null dereference ends the process and the run reports a single packet. With it,
 * one run classifies the whole suite. Installed and removed around the run (see FaultTranslator) so
 * it never changes fault semantics for the rest of the server thread when this runs in-game.
 */
void translateStructuredException(unsigned int code, struct _EXCEPTION_POINTERS *)
{
    // Spelled out rather than including <windows.h>, whose min/max macros collide with ported field
    // names such as Attribute::min.
    constexpr unsigned int ACCESS_VIOLATION = 0xC0000005;
    throw std::runtime_error(code == ACCESS_VIOLATION ? "access violation (a required member was null)"
                                                      : "structured exception " + std::to_string(code));
}

class FaultTranslator {
public:
    FaultTranslator() : previous_(_set_se_translator(translateStructuredException)) {}
    ~FaultTranslator() { _set_se_translator(previous_); }
    FaultTranslator(const FaultTranslator &) = delete;
    FaultTranslator &operator=(const FaultTranslator &) = delete;

private:
    _se_translator_function previous_;
};
#else
class FaultTranslator {};
#endif

/** Independent of ValueWell, so fuzzing cannot perturb the field values under test. */
class FuzzRandom {
public:
    explicit FuzzRandom(std::uint64_t seed) : state_(seed | 1ULL) {}

    std::uint64_t next()
    {
        state_ += 0x9E3779B97F4A7C15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    std::size_t below(std::size_t bound) { return bound == 0 ? 0 : static_cast<std::size_t>(next() % bound); }

private:
    std::uint64_t state_;
};

std::unique_ptr<DataPacket> freshInstance(std::uint32_t pid)
{
    return PacketPool::getInstance().getPacketById(pid);
}

/** Encodes into a fresh buffer; the caller decides how to classify a throw. */
std::string encodeBodyOf(const DataPacket &packet)
{
    ByteBufferWriter out(256);
    packet.encodeBody(out);
    return out.getData();
}

void recordDiff(PacketResult &result, std::string_view expected, std::string_view actual)
{
    result.hasDiff = true;
    result.diffOffset = firstDifference(expected, actual);
    result.expectedSize = expected.size();
    result.actualSize = actual.size();
    result.expectedWindow = hexWindow(expected, result.diffOffset);
    result.actualWindow = hexWindow(actual, result.diffOffset);
    result.hint = explainMismatch(expected, actual, result.diffOffset);
}

/**
 * Decodes a deliberately damaged buffer and reports only what must never happen.
 *
 * Acceptable: throwing anything derived from std::exception, or decoding cleanly (a shorter buffer
 * can be a legal shorter packet). Unacceptable: a hardware fault, or a throw that is NOT derived from
 * std::exception - the latter would escape PacketInterceptor::dispatch's catch and kill the server.
 *
 * Returns an empty string when the buffer behaved, or the reason it did not.
 */
std::string probeDamagedBuffer(std::uint32_t pid, std::string_view damaged)
{
    auto packet = freshInstance(pid);
    if (packet == nullptr) {
        return {};
    }

    ByteBufferReader in(damaged);
    try {
        packet->decodeBody(in);
    }
    catch (const std::exception &) {
        return {};  // the intended failure mode
    }
    catch (...) {
        return "threw a non-std::exception, which would escape the bridge's catch and kill the server";
    }

    if (in.getUnreadLength() != 0) {
        return {};  // decoded a prefix and stopped; the bridge treats this as a decode failure
    }

    // It decoded cleanly, so the bridge would happily re-encode it. That path must be stable too.
    try {
        const std::string once = encodeBodyOf(*packet);
        auto again = freshInstance(pid);
        ByteBufferReader reread(once);
        again->decodeBody(reread);
        if (reread.getUnreadLength() != 0 || encodeBodyOf(*again) != once) {
            return "a damaged buffer decoded cleanly but did not re-encode stably";
        }
    }
    catch (const std::exception &) {
        return "a damaged buffer decoded cleanly but then failed to re-encode";
    }
    catch (...) {
        return "a damaged buffer decoded cleanly but re-encoding threw a non-std::exception";
    }
    return {};
}

/**
 * @param canProvokeFaults whether a hardware fault would be reported rather than ending the process.
 *        False on toolchains without _set_se_translator (clang-cl), which is how the plugin is built.
 */
PacketResult checkPacket(const SelectedPacket &entry, ValueWell &well, const Options &options, bool fuzz,
                         bool canProvokeFaults)
{
    PacketResult result;
    result.pid = entry.pid;
    result.name = entry.name;
    result.tier = entry.tier;
    result.declaredFields = entry.declaredFields;

    const auto fail = [&result](Stage stage, std::string detail) -> PacketResult & {
        result.failedAt = stage;
        result.detail = std::move(detail);
        return result;
    };

    // ---- S0  build a fully populated instance -----------------------------------------------
    well.beginPacket(entry.pid);
    FillContext context{well};
    std::unique_ptr<DataPacket> packet;
    try {
        packet = entry.fill(context);
    }
    catch (const std::exception &e) {
        return fail(Stage::Fill, e.what());
    }
    catch (...) {
        return fail(Stage::Fill, "the filler threw a non-std::exception");
    }
    if (packet == nullptr) {
        return fail(Stage::Fill, "the filler returned null");
    }
    if (packet->pid() != entry.pid) {
        return fail(Stage::Fill, "the filler built packet ID " + std::to_string(packet->pid()) + ", expected " +
                                     std::to_string(entry.pid));
    }

    // ---- S1  encode -------------------------------------------------------------------------
    std::string filled;
    try {
        filled = encodeBodyOf(*packet);
    }
    catch (const std::exception &e) {
        return fail(Stage::Encode, e.what());
    }
    catch (...) {
        return fail(Stage::Encode, "encodeBody threw a non-std::exception");
    }
    result.bodySize = filled.size();

    // ---- S2  anti-tautology -----------------------------------------------------------------
    // If a fully populated packet encodes to the same bytes as a default-constructed one, the filler
    // did nothing and every stage after this would pass without testing anything.
    //
    // This is the ONLY stage that deliberately touches a default-constructed packet, and a handful of
    // them dereference a null required member (the same is true in PHP). That is a hardware fault, so
    // the stage can only run where faults are catchable. Without the translator it would not report a
    // failure - it would take the whole server down, which is exactly what happened in-game before
    // this guard existed.
    if (!canProvokeFaults) {
        result.antiTautologyRun = false;
    }
    else if (auto blank = freshInstance(entry.pid); blank != nullptr) {
        try {
            const std::string blankBytes = encodeBodyOf(*blank);
            result.defaultEncodable = true;
            if (blankBytes == filled) {
                return fail(Stage::AntiTautology,
                            "the filled packet encodes to the same bytes as a default-constructed one, so "
                            "the filler is not actually populating anything");
            }
        }
        catch (...) {
            // Cannot encode from its default state - the same is true in PHP, where the required
            // member is an uninitialised typed property. Nothing to compare against, so the check is
            // vacuous rather than failed; the report says so.
            result.defaultEncodable = false;
        }
    }

    // ---- S3/S4  decode our own output, consuming all of it ----------------------------------
    auto decoded = freshInstance(entry.pid);
    if (decoded == nullptr) {
        return fail(Stage::Decode, "PacketPool has no factory for this ID");
    }
    ByteBufferReader in(filled);
    try {
        decoded->decodeBody(in);
    }
    catch (const std::exception &e) {
        return fail(Stage::Decode, e.what());
    }
    catch (...) {
        return fail(Stage::Decode, "decodeBody threw a non-std::exception");
    }
    if (in.getUnreadLength() != 0) {
        return fail(Stage::Drain, std::to_string(in.getUnreadLength()) + " of " + std::to_string(filled.size()) +
                                      " bytes were left unread, so decodePayload is not reading every field "
                                      "encodePayload writes");
    }

    // ---- S5  re-encode byte-identically -----------------------------------------------------
    // This is the stage that matters. An asymmetry here is exactly what silently corrupts packets
    // when the bridge re-encodes a modified packet on a live server.
    std::string second;
    try {
        second = encodeBodyOf(*decoded);
    }
    catch (const std::exception &e) {
        return fail(Stage::ReEncode, e.what());
    }
    if (second != filled) {
        recordDiff(result, filled, second);
        return fail(Stage::ReEncode, "decode/encode are not symmetric for this packet");
    }

    // ---- S6  idempotence --------------------------------------------------------------------
    auto third = freshInstance(entry.pid);
    try {
        ByteBufferReader secondIn(second);
        third->decodeBody(secondIn);
        if (secondIn.getUnreadLength() != 0) {
            return fail(Stage::Idempotence, "the second generation left trailing bytes");
        }
        const std::string thirdBytes = encodeBodyOf(*third);
        if (thirdBytes != second) {
            recordDiff(result, second, thirdBytes);
            return fail(Stage::Idempotence, "generation 2 and 3 disagree, so the first round-trip was lossy "
                                            "in a way the first comparison happened to hide");
        }
    }
    catch (const std::exception &e) {
        return fail(Stage::Idempotence, e.what());
    }

    // ---- S7  the header path ----------------------------------------------------------------
    // encode()/decode() carry the packet ID and the split-screen sub-client IDs. Nothing tested that
    // path before, and the bridge relies on it whenever it injects a packet.
    try {
        packet->senderSubId = 2;
        packet->recipientSubId = 3;

        ByteBufferWriter headerOut(filled.size() + 8);
        packet->encode(headerOut);
        const std::string withHeader = headerOut.getData();

        if (withHeader.size() < filled.size() ||
            withHeader.compare(withHeader.size() - filled.size(), filled.size(), filled) != 0) {
            recordDiff(result, filled, withHeader);
            return fail(Stage::HeaderPath, "encode() did not produce the header followed by exactly the same "
                                           "body encodeBody() produces");
        }

        auto headerDecoded = freshInstance(entry.pid);
        ByteBufferReader headerIn(withHeader);
        headerDecoded->decode(headerIn);

        if (headerIn.getUnreadLength() != 0) {
            return fail(Stage::HeaderPath, "decode() left trailing bytes");
        }
        if (headerDecoded->senderSubId != 2 || headerDecoded->recipientSubId != 3) {
            return fail(Stage::HeaderPath, "the sub-client IDs did not survive the header round-trip (got " +
                                               std::to_string(headerDecoded->senderSubId) + "/" +
                                               std::to_string(headerDecoded->recipientSubId) + ", expected 2/3)");
        }

        ByteBufferWriter headerAgain(withHeader.size());
        headerDecoded->encode(headerAgain);
        if (headerAgain.getData() != withHeader) {
            recordDiff(result, withHeader, headerAgain.getData());
            return fail(Stage::HeaderPath, "the header path did not re-encode byte-identically");
        }
    }
    catch (const std::exception &e) {
        return fail(Stage::HeaderPath, e.what());
    }

    if (!fuzz) {
        result.passed = true;
        return result;
    }

    // ---- S9  truncation ---------------------------------------------------------------------
    // Every prefix of a valid packet is something a drifted or hostile peer can send. None of them
    // may fault.
    {
        const std::size_t size = filled.size();
        // Short bodies are covered exhaustively. Long ones take the first 256 prefixes (where the
        // header-shaped fields live) plus an even sample of the rest, and the report says which.
        const bool exhaustive = size <= 4096;
        const std::size_t stride = exhaustive ? 1 : std::max<std::size_t>(1, size / 256);
        for (std::size_t length = 0; length < size; length += (exhaustive || length < 256) ? 1 : stride) {
            ++result.truncationPrefixes;
            const std::string reason = probeDamagedBuffer(entry.pid, std::string_view(filled).substr(0, length));
            if (!reason.empty()) {
                return fail(Stage::Truncation, "truncated to " + std::to_string(length) + " bytes: " + reason);
            }
        }
    }

    // ---- S10  corruption --------------------------------------------------------------------
    if (!filled.empty()) {
        FuzzRandom random(options.seed ^ (static_cast<std::uint64_t>(entry.pid) << 32));
        for (int pass = 0; pass < options.corruptionPasses; ++pass) {
            std::string damaged = filled;
            if ((pass & 1) == 0) {
                // Bit flips: the smallest change that still reaches a different decode branch.
                const std::size_t flips = 1 + random.below(3);
                for (std::size_t i = 0; i < flips; ++i) {
                    const std::size_t index = random.below(damaged.size());
                    damaged[index] = static_cast<char>(static_cast<unsigned char>(damaged[index]) ^
                                                       (1u << random.below(8)));
                }
            }
            else {
                // 0xFF maximises varint length and produces the largest possible lengths and counts;
                // 0x00 and 0x7F terminate varints early. Between them they hit the interesting edges.
                static constexpr unsigned char POISON[] = {0xFF, 0x00, 0x7F};
                const std::size_t writes = 1 + random.below(4);
                for (std::size_t i = 0; i < writes; ++i) {
                    const std::size_t index = random.below(damaged.size());
                    damaged[index] = static_cast<char>(POISON[random.below(3)]);
                }
            }
            ++result.corruptionPasses;
            const std::string reason = probeDamagedBuffer(entry.pid, damaged);
            if (!reason.empty()) {
                return fail(Stage::Corruption, "corruption pass " + std::to_string(pass) + ": " + reason);
            }
        }
    }

    result.passed = true;
    return result;
}

}  // namespace

Report runDeepRoundTrip(const Options &options)
{
    Report report;
    report.seed = options.seed;
    report.faultTranslationAvailable = BP_VERIFY_HAS_SE_TRANSLATOR != 0;
    // Fuzzing without fault translation would take the process down on the first defect instead of
    // reporting it, which is worse than not running it.
    report.fuzzEnabled = options.fuzz && report.faultTranslationAvailable;

    // Installed for the duration of the run and removed on the way out, so it never changes fault
    // semantics for the rest of the server thread when the suite runs in-game. On a toolchain
    // without the translator this is an empty object, hence [[maybe_unused]].
    [[maybe_unused]] const FaultTranslator translator;
    ValueWell well(options.seed);

    for (const auto &entry : getSelection()) {
        if (!options.only.empty()) {
            const auto *wanted = findSelectedByName(options.only);
            if (wanted == nullptr || wanted->pid != entry.pid) {
                continue;
            }
        }
        report.results.push_back(
            checkPacket(entry, well, options, report.fuzzEnabled, report.faultTranslationAvailable));
    }

    report.duplicateValues = well.getDuplicateCount();
    return report;
}

}  // namespace bedrock_protocol::verify
