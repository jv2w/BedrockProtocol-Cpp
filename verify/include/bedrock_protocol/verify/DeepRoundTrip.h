/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * The offline deep round-trip engine.
 *
 * What it adds over tests/PacketRoundTripTest.cpp: that suite encodes DEFAULT-constructed packets,
 * so every field is 0 and a field swap, a wrong integer width or an unread field all round-trip
 * perfectly. This one fills every field with a distinct, deliberately-shaped value first, which is
 * what makes those defects change the bytes.
 *
 * The engine returns data and prints nothing, so the offline test binary and the in-game /proto
 * selftest run exactly the same code rather than two implementations that can drift.
 */

#pragma once

#include <cstdint>
#include <string>

#include "bedrock_protocol/verify/Report.h"

namespace bedrock_protocol::verify {

struct Options {
    /** Any fixed value works; it is echoed in the report so a failure reproduces exactly. */
    std::uint64_t seed = 0x5DEECE66DULL;

    /** Restrict the run to one packet, by class name. Empty means the whole suite. */
    std::string only;

    /**
     * Run the truncation and corruption stages.
     *
     * These are what protect the live server: a version-drifted or hostile client packet must never
     * take BDS down through this library. They need hardware-fault translation to distinguish "threw
     * cleanly" from "faulted", and are skipped with a note in the report when it is unavailable.
     */
    bool fuzz = true;

    /** Corruption passes per packet. Each pass flips bits or overwrites bytes, then re-decodes. */
    int corruptionPasses = 64;
};

Report runDeepRoundTrip(const Options &options);

}  // namespace bedrock_protocol::verify
