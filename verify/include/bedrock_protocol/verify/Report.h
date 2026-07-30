/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * The result of a deep round-trip run.
 *
 * runDeepRoundTrip() returns this and prints nothing. Rendering lives here instead, because the same
 * run has to reach three very different places: a console, in-game chat, and a form. Keeping the
 * engine free of I/O is what lets the plugin reuse it verbatim rather than growing a second copy.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "bedrock_protocol/verify/Filler.h"

namespace bedrock_protocol::verify {

/** Where a packet stopped. Ordered as the engine runs them, so the enum doubles as progress. */
enum class Stage {
    Fill,            ///< S0  the filler itself threw (or faulted, via the SE translator)
    Encode,          ///< S1  encodeBody threw
    AntiTautology,   ///< S2  filled bytes equal default-constructed bytes: the filler is a no-op
    Decode,          ///< S3  decodeBody threw on our own output
    Drain,           ///< S4  decode left trailing bytes
    ReEncode,        ///< S5  re-encode produced different bytes  <- the defect that corrupts the wire
    Idempotence,     ///< S6  generation 2 and 3 disagree
    HeaderPath,      ///< S7  encode()/decode() with sub-client IDs disagrees with encodeBody()
    Truncation,      ///< S9  a truncated buffer faulted or threw something uncatchable
    Corruption,      ///< S10 a corrupted buffer faulted, or decoded and then re-encoded unstably
    Passed,
};

std::string_view toString(Stage stage);

struct PacketResult {
    std::uint32_t pid = 0;
    std::string name;
    Tier tier = Tier::Rare;
    int declaredFields = 0;
    std::size_t bodySize = 0;

    bool passed = false;
    Stage failedAt = Stage::Passed;
    std::string detail;

    /**
     * True when the default-constructed packet could not be encoded at all.
     *
     * Those are the packets PHP also cannot encode from their default state (a required member is an
     * uninitialised typed property there). The anti-tautology comparison has nothing to compare
     * against, so it is vacuous rather than failed - but it is reported, so a filler cannot hide
     * behind it.
     */
    bool defaultEncodable = true;

    /**
     * False when the anti-tautology comparison could not be attempted at all.
     *
     * It requires encoding a default-constructed packet, which for a few packets is a hardware
     * fault; that is only survivable where faults are catchable. Reported rather than silently
     * skipped, so a green in-game run is not mistaken for the full gate.
     */
    bool antiTautologyRun = true;

    /** Byte-level context, populated only for a byte mismatch. */
    bool hasDiff = false;
    std::size_t diffOffset = 0;
    std::size_t expectedSize = 0;
    std::size_t actualSize = 0;
    std::string expectedWindow;
    std::string actualWindow;
    std::string hint;

    /** Fuzz counters, so the report can show the work rather than merely asserting it was done. */
    std::size_t truncationPrefixes = 0;
    std::size_t corruptionPasses = 0;
};

struct Report {
    std::uint64_t seed = 0;
    std::vector<PacketResult> results;
    int duplicateValues = 0;
    bool fuzzEnabled = false;
    bool faultTranslationAvailable = false;

    [[nodiscard]] int total() const { return static_cast<int>(results.size()); }
    [[nodiscard]] int passedCount() const;
    [[nodiscard]] int fieldsCovered() const;
    [[nodiscard]] std::size_t truncationPrefixes() const;
    [[nodiscard]] std::size_t corruptionPasses() const;
    [[nodiscard]] bool ok() const { return passedCount() == total() && duplicateValues == 0; }

    /** Full console rendering, including a block per failure. */
    [[nodiscard]] std::string toPlainText() const;

    /** Compact, colour-coded lines for in-game chat. Bounded length; failures win over summary. */
    [[nodiscard]] std::vector<std::string> toChatLines() const;
};

/** Renders a byte range around `offset` as hex, bracketing the byte at `offset`. */
std::string hexWindow(std::string_view bytes, std::size_t offset, std::size_t radius = 8);

/** Index of the first differing byte, or the length of the shorter string if one is a prefix. */
std::size_t firstDifference(std::string_view a, std::string_view b);

/**
 * Turns a byte mismatch into a sentence naming the likely cause.
 *
 * A hex diff tells you where; this tells you what kind of mistake produces that shape. The rules are
 * heuristics and say so - they point at the suspect, they do not convict it.
 */
std::string explainMismatch(std::string_view expected, std::string_view actual, std::size_t offset);

}  // namespace bedrock_protocol::verify
