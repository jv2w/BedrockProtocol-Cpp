/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * MALFORMED INPUT ROBUSTNESS SUITE.
 *
 * The deep round-trip suite fuzzes shapes derived from VALID packets, and only the packets that have
 * fillers. This suite is the complement: it feeds purely hostile, never-valid buffers to EVERY
 * registered packet ID and asserts nothing more than "the process survived and the failure was a
 * std::exception".
 *
 * That is exactly the contract bridge::PacketInterceptor::dispatch relies on. dispatch decodes
 * attacker-controlled bytes on the server thread behind a single `catch (const std::exception &)`,
 * so a decoder that faults, throws something not derived from std::exception, or simply takes a long
 * time is a remote kill or stall of a public server.
 *
 * Built with MSVC /EHa so _set_se_translator can turn a hardware fault into a reportable FAULTED
 * result instead of a dead process. See tools/run_malformed_test.ps1.
 */

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <new>
#include <string>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/protocol/PacketPool.h"

#if defined(_MSC_VER) && !defined(__clang__)
#define BP_HAS_SE_TRANSLATOR 1
#include <eh.h>
#else
#define BP_HAS_SE_TRANSLATOR 0
#endif

// ---------------------------------------------------------------------------------------------
// Allocation accounting
//
// Replacing the global operator new lets us see every allocation the prebuilt core library makes,
// which is the only portable-enough way to catch "attacker-supplied count reaches reserve()".
//
// The hard cap is a HARNESS guard, not a library change: without it a single 8 GiB reserve() would
// either take the test machine down or succeed silently and hide the defect. With it we record the
// requested size and hand back std::bad_alloc, which is what an out-of-memory box would do anyway.
// ---------------------------------------------------------------------------------------------

namespace {

constexpr std::size_t kAllocHardCap = std::size_t{1} << 28;  // 256 MiB
constexpr std::size_t kAllocSuspicious = std::size_t{1} << 22;  // 4 MiB

bool g_allocTracking = false;
std::size_t g_allocLargestRequest = 0;
std::size_t g_allocTotalRequested = 0;
bool g_allocCapHit = false;

inline void *trackedAllocate(std::size_t n)
{
    if (g_allocTracking) {
        if (n > g_allocLargestRequest) {
            g_allocLargestRequest = n;
        }
        g_allocTotalRequested += n;
        if (n > kAllocHardCap) {
            g_allocCapHit = true;
            throw std::bad_alloc();
        }
    }
    void *p = std::malloc(n != 0 ? n : 1);
    if (p == nullptr) {
        throw std::bad_alloc();
    }
    return p;
}

}  // namespace

void *operator new(std::size_t n) { return trackedAllocate(n); }
void *operator new[](std::size_t n) { return trackedAllocate(n); }
void *operator new(std::size_t n, std::align_val_t) { return trackedAllocate(n); }
void *operator new[](std::size_t n, std::align_val_t) { return trackedAllocate(n); }
void operator delete(void *p) noexcept { std::free(p); }
void operator delete[](void *p) noexcept { std::free(p); }
void operator delete(void *p, std::size_t) noexcept { std::free(p); }
void operator delete[](void *p, std::size_t) noexcept { std::free(p); }
void operator delete(void *p, std::align_val_t) noexcept { std::free(p); }
void operator delete[](void *p, std::align_val_t) noexcept { std::free(p); }
void operator delete(void *p, std::size_t, std::align_val_t) noexcept { std::free(p); }
void operator delete[](void *p, std::size_t, std::align_val_t) noexcept { std::free(p); }

namespace {

using bedrock_protocol::PacketPool;
using bedrock_protocol::encoding::ByteBufferReader;
using Clock = std::chrono::steady_clock;

constexpr double kTimeBudgetMs = 50.0;

#if BP_HAS_SE_TRANSLATOR
/** Thrown in place of a hardware fault so one fault classifies a case instead of ending the run. */
struct HardwareFault : std::exception {
    explicit HardwareFault(unsigned int code) : code(code) {}
    [[nodiscard]] const char *what() const noexcept override { return "hardware fault"; }
    unsigned int code;
};

void translateStructuredException(unsigned int code, struct _EXCEPTION_POINTERS *)
{
    throw HardwareFault(code);
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

/** SplitMix64. Deterministic across runs and platforms so every finding is reproducible. */
class Random {
public:
    explicit Random(std::uint64_t seed) : state_(seed) {}

    std::uint64_t next()
    {
        state_ += 0x9E3779B97F4A7C15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    std::uint8_t nextByte() { return static_cast<std::uint8_t>(next() & 0xFF); }

private:
    std::uint64_t state_;
};

struct Case {
    std::string name;
    std::string data;
};

std::string repeated(std::uint8_t b, std::size_t n)
{
    return std::string(n, static_cast<char>(b));
}

std::string alternating(std::uint8_t a, std::uint8_t b, std::size_t n)
{
    std::string s(n, '\0');
    for (std::size_t i = 0; i < n; ++i) {
        s[i] = static_cast<char>((i % 2 == 0) ? a : b);
    }
    return s;
}

/**
 * Network-NBT compound nested `depth` levels deep.
 *
 * Root is `TAG_Compound, name=""`; every level then opens another `TAG_Compound` child named "",
 * and the whole thing is closed with `depth` TAG_End bytes. This is the canonical stack-exhaustion
 * vector - NBT::createTag recurses once per level - so it exists to prove ReaderTracker's depth
 * limit actually fires before the stack runs out.
 */
std::string nestedCompound(std::size_t depth)
{
    std::string s;
    s.reserve(depth * 2 + depth + 2);
    s += '\x0A';  // root TAG_Compound
    s += '\x00';  // root name: varint length 0
    for (std::size_t i = 0; i < depth; ++i) {
        s += '\x0A';  // child TAG_Compound
        s += '\x00';  // child name: varint length 0
    }
    s += std::string(depth + 1, '\x00');  // TAG_End for every open compound
    return s;
}

/**
 * Network-NBT list nested `depth` levels deep, each list holding exactly one child list.
 *
 * Lists need no terminator, so this is a denser recursion driver than nestedCompound: two bytes per
 * level with no closing run at all. Size 1 is written as zigzag varint 0x02.
 */
std::string nestedList(std::size_t depth)
{
    std::string s;
    s.reserve(depth * 2 + 2);
    s += '\x09';  // root TAG_List
    s += '\x00';  // root name: varint length 0
    for (std::size_t i = 0; i < depth; ++i) {
        s += '\x09';  // element type TAG_List
        s += '\x02';  // element count: zigzag varint for 1
    }
    s += '\x00';  // innermost: TAG_End element type
    s += '\x00';  // innermost: count 0
    return s;
}

/**
 * A chain of nested cereal DynamicValue lists, each holding exactly one element.
 *
 * The cereal union recurses through DynamicValue::read the same way NBT recurses through its tags,
 * and it is even cheaper to drive: five bytes per level (a one-byte count of 1 and the four-byte
 * little-endian type of the single element). A 50 KB packet therefore asks for ten thousand frames.
 * The generic byte-pattern cases cannot reach this - the type word has to be a valid LIST id at
 * every level - so the nesting bound needs a case of its own.
 */
std::string nestedDynamicValueList(std::size_t depth)
{
    constexpr char kList = '\x05';  // cereal::DynamicValueType::LIST
    std::string s;
    s.reserve(depth * 5 + 16);

    // The chain is only reached through a well-formed ClientboundDataStorePacket prefix: one CHANGE
    // entry, its two empty strings, its update count, and the type of the value that follows. The
    // generic byte patterns cannot produce this, which is why the recursion went unexercised.
    s += '\x01';  // entry count: unsigned varint 1
    s += '\x01';  // operation: unsigned varint DataStoreOperationType::CHANGE
    s += '\x00';  // name: empty string
    s += '\x00';  // property: empty string
    s.append(4, '\x00');  // update count: uint32 LE
    s += kList;           // value type: uint32 LE, low byte first
    s.append(3, '\x00');

    for (std::size_t i = 0; i < depth; ++i) {
        s += '\x01';  // element count: unsigned varint 1
        s += kList;   // element type: uint32 LE, low byte first
        s.append(3, '\x00');
    }
    s += '\x00';  // innermost list: count 0
    return s;
}

/**
 * A single tag of the given type, carrying a declared length of INT32_MAX, wrapped in a compound.
 *
 * The wrapper is essential: every protocol entry point demands a TAG_Compound root (getNbtRoot ->
 * mustGetCompoundTag, or readHeadless(TAG_Compound)), so a bare TAG_Int_Array root is rejected on the
 * type check and the array-length code is never reached. Nesting it one level inside a compound is
 * what actually drives readIntArray()/readByteArray().
 *
 * This is the direct probe for the reserve()-on-wire-length defect: the length is read before a
 * single element is, so a decoder that reserves on it commits ~8 GiB from ~10 bytes of input.
 */
std::string hugeArrayInCompound(char tagType)
{
    std::string s;
    s += '\x0A';  // root TAG_Compound
    s += '\x00';  // root name: varint length 0
    s += tagType; // member tag
    s += '\x00';  // member name: varint length 0
    // zigzag(INT32_MAX) = 0xFFFFFFFE, LEB128 -> FE FF FF FF 0F
    s += '\xFE';
    s += '\xFF';
    s += '\xFF';
    s += '\xFF';
    s += '\x0F';
    s += '\x00';  // TAG_End closing the root compound
    return s;
}

/**
 * The corpus fed to every packet.
 *
 * Shared across packets on purpose: we do not know which packets carry NBT or which offsets their
 * fields land at, so every hostile shape is offered to every decoder, including at a range of
 * leading offsets so an NBT field that sits behind a few scalar fields still gets reached.
 */
std::vector<Case> buildCorpus()
{
    std::vector<Case> cases;

    cases.push_back({"empty", ""});

    for (std::size_t n : {std::size_t{1}, std::size_t{2}, std::size_t{4}, std::size_t{8}, std::size_t{16},
                          std::size_t{64}, std::size_t{256}, std::size_t{1024}}) {
        cases.push_back({"zeros[" + std::to_string(n) + "]", repeated(0x00, n)});
    }

    // 0xFF maximises every varint: unsigned varints decode to their largest value and signed
    // (zigzag) varints to their most negative, so counts and lengths are as hostile as the wire
    // format permits.
    for (std::size_t n : {std::size_t{1}, std::size_t{2}, std::size_t{4}, std::size_t{5}, std::size_t{8},
                          std::size_t{16}, std::size_t{64}, std::size_t{256}, std::size_t{1024},
                          std::size_t{4096}}) {
        cases.push_back({"ff[" + std::to_string(n) + "]", repeated(0xFF, n)});
    }

    // 0x80 keeps a varint continuing, 0x7F terminates it: a stream of maximal-but-valid varints.
    for (std::size_t n : {std::size_t{16}, std::size_t{64}, std::size_t{256}, std::size_t{1024}}) {
        cases.push_back({"alt80_7f[" + std::to_string(n) + "]", alternating(0x80, 0x7F, n)});
    }
    for (std::size_t n : {std::size_t{64}, std::size_t{256}}) {
        cases.push_back({"alt_ff00[" + std::to_string(n) + "]", alternating(0xFF, 0x00, n)});
        cases.push_back({"alt_7f80[" + std::to_string(n) + "]", alternating(0x7F, 0x80, n)});
    }

    // 0x80 repeated is a varint that never terminates: probes unterminated-varint handling.
    for (std::size_t n : {std::size_t{16}, std::size_t{256}}) {
        cases.push_back({"cont80[" + std::to_string(n) + "]", repeated(0x80, n)});
    }

    Random rng(0xB3DA0C4F5EED1234ULL);
    for (std::size_t n : {std::size_t{1}, std::size_t{3}, std::size_t{7}, std::size_t{16}, std::size_t{32},
                          std::size_t{64}, std::size_t{128}, std::size_t{512}}) {
        for (int rep = 0; rep < 4; ++rep) {
            std::string s(n, '\0');
            for (std::size_t i = 0; i < n; ++i) {
                s[i] = static_cast<char>(rng.nextByte());
            }
            cases.push_back({"rand" + std::to_string(n) + "#" + std::to_string(rep), std::move(s)});
        }
    }

    // NBT-shaped payloads, each offered raw and behind 1/2/4/8 leading zero bytes so it still lands
    // on an NBT field when the packet has scalar fields in front of it.
    std::vector<std::pair<std::string, std::string>> nbt = {
        {"nbt_compound_d64", nestedCompound(64)},     {"nbt_compound_d512", nestedCompound(512)},
        {"nbt_compound_d100k", nestedCompound(100000)}, {"nbt_list_d512", nestedList(512)},
        {"nbt_list_d100k", nestedList(100000)},
        {"nbt_intarray_max", hugeArrayInCompound('\x0B')},
        {"nbt_bytearray_max", hugeArrayInCompound('\x07')},
        {"nbt_longarray_max", hugeArrayInCompound('\x0C')},
        {"nbt_string_max", hugeArrayInCompound('\x08')},
        {"cereal_list_d512", nestedDynamicValueList(512)},
        {"cereal_list_d100k", nestedDynamicValueList(100000)},
    };
    for (const auto &[name, body] : nbt) {
        for (std::size_t pad : {std::size_t{0}, std::size_t{1}, std::size_t{2}, std::size_t{4}, std::size_t{8}}) {
            cases.push_back({name + "+pad" + std::to_string(pad), repeated(0x00, pad) + body});
        }
    }

    return cases;
}

enum class Outcome { Clean, ThrewStd, Faulted, ThrewNonStd, OverBudget };

struct Finding {
    std::string packet;
    std::uint32_t pid;
    std::string caseName;
    Outcome outcome;
    double ms;
    std::size_t largestAlloc;
    std::size_t totalAlloc;
    bool capHit;
    std::string detail;
};

std::string hexPreview(const std::string &s)
{
    std::string out;
    const std::size_t n = std::min<std::size_t>(s.size(), 24);
    char buf[8];
    for (std::size_t i = 0; i < n; ++i) {
        std::snprintf(buf, sizeof(buf), "%02X", static_cast<unsigned char>(s[i]));
        out += buf;
    }
    if (s.size() > n) {
        out += "... (" + std::to_string(s.size()) + " bytes)";
    }
    return out;
}

}  // namespace

namespace {
/**
 * Prints every case before it runs.
 *
 * The one failure mode this suite cannot catch is stack exhaustion: _set_se_translator has no stack
 * left to throw from, so the process dies. -v is how you find which case did it - the last line
 * printed is the culprit.
 */
bool g_verbose = false;
}  // namespace

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-v") == 0) {
            g_verbose = true;
        }
    }

    // Unbuffered: if a case does manage to kill the process, the last line printed names it.
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    FaultTranslator translator;

    const auto corpus = buildCorpus();
    const auto &pool = PacketPool::getInstance();

    std::vector<Finding> criticals;
    std::vector<Finding> slowest;
    std::vector<Finding> greediest;

    std::size_t packetCount = 0;
    std::size_t decodeCount = 0;
    std::size_t threwStd = 0;
    std::size_t clean = 0;

    std::printf("Malformed input robustness suite\n");
#if BP_HAS_SE_TRANSLATOR
    std::printf("  hardware-fault detection: ENABLED (_set_se_translator)\n");
#else
    std::printf("  hardware-fault detection: DISABLED - a fault will kill the process\n");
#endif
    std::printf("  corpus cases per packet : %zu\n", corpus.size());
    std::printf("  time budget per decode  : %.1f ms\n", kTimeBudgetMs);
    std::printf("  allocation hard cap     : %zu bytes (harness guard)\n\n", kAllocHardCap);

    for (std::uint32_t pid = 0; pid < 512; ++pid) {
        auto probe = pool.getPacketById(pid);
        if (!probe) {
            continue;
        }
        ++packetCount;
        const std::string name(probe->getName());

        for (const auto &c : corpus) {
            // A fresh instance per case: a half-populated packet must never influence the next case.
            auto packet = pool.getPacketById(pid);
            ByteBufferReader in(c.data);

            Outcome outcome = Outcome::Clean;
            std::string detail;

            g_allocLargestRequest = 0;
            g_allocTotalRequested = 0;
            g_allocCapHit = false;

            if (g_verbose) {
                std::printf("... pid=%u %s case=%s\n", pid, name.c_str(), c.name.c_str());
            }

            const auto start = Clock::now();
            g_allocTracking = true;
            try {
                packet->decodeBody(in);
                outcome = Outcome::Clean;
            }
#if BP_HAS_SE_TRANSLATOR
            catch (const HardwareFault &e) {
                outcome = Outcome::Faulted;
                char buf[64];
                std::snprintf(buf, sizeof(buf), "SEH code 0x%08X", e.code);
                detail = buf;
            }
#endif
            catch (const std::exception &e) {
                outcome = Outcome::ThrewStd;
                detail = e.what();
            }
            catch (...) {
                outcome = Outcome::ThrewNonStd;
                detail = "non-std exception";
            }
            g_allocTracking = false;
            const double ms = std::chrono::duration<double, std::milli>(Clock::now() - start).count();

            const std::size_t largest = g_allocLargestRequest;
            const std::size_t total = g_allocTotalRequested;
            const bool capHit = g_allocCapHit;
            ++decodeCount;

            Finding f{name, pid, c.name, outcome, ms, largest, total, capHit, detail};

            if (outcome == Outcome::Faulted || outcome == Outcome::ThrewNonStd) {
                criticals.push_back(f);
            }
            else if (ms > kTimeBudgetMs) {
                f.outcome = Outcome::OverBudget;
                criticals.push_back(f);
            }
            else if (outcome == Outcome::ThrewStd) {
                ++threwStd;
            }
            else {
                ++clean;
            }

            slowest.push_back(f);
            if (capHit || largest > kAllocSuspicious) {
                greediest.push_back(f);
            }
        }
    }

    std::printf("registered packet IDs exercised : %zu\n", packetCount);
    std::printf("total decodes                   : %zu\n", decodeCount);
    std::printf("  threw std::exception (good)   : %zu\n", threwStd);
    std::printf("  returned cleanly (fine)       : %zu\n", clean);
    std::printf("  CRITICAL                      : %zu\n\n", criticals.size());

    if (!criticals.empty()) {
        std::printf("=== CRITICAL ===\n");
        for (const auto &f : criticals) {
            const char *kind = f.outcome == Outcome::Faulted        ? "FAULTED"
                               : f.outcome == Outcome::ThrewNonStd  ? "THREW-NON-STD"
                                                                    : "OVER-TIME-BUDGET";
            std::printf("  [%s] pid=%u %s case=%s %.2f ms largestAlloc=%zu\n", kind, f.pid, f.packet.c_str(),
                        f.caseName.c_str(), f.ms, f.largestAlloc);
            if (!f.detail.empty()) {
                std::printf("      %s\n", f.detail.c_str());
            }
        }
        std::printf("\n");
    }

    std::sort(slowest.begin(), slowest.end(), [](const Finding &a, const Finding &b) { return a.ms > b.ms; });
    std::printf("=== 15 slowest decodes ===\n");
    for (std::size_t i = 0; i < slowest.size() && i < 15; ++i) {
        const auto &f = slowest[i];
        std::printf("  %8.3f ms  pid=%-4u %-38s %s\n", f.ms, f.pid, f.packet.c_str(), f.caseName.c_str());
    }
    std::printf("\n");

    std::sort(greediest.begin(), greediest.end(),
              [](const Finding &a, const Finding &b) { return a.largestAlloc > b.largestAlloc; });
    // A large allocation is only a DEFECT when it is unbounded - that is, when a decoder asked for
    // more than the input could possibly justify and hit the harness cap. A merely large request that
    // stays proportional to the input (a vector of structs grown from a count where every element
    // consumes wire bytes) is reported for the record but is not a failure.
    std::size_t capHits = 0;
    for (const auto &f : greediest) {
        if (f.capHit) {
            ++capHits;
        }
    }
    std::printf("=== allocation outliers (single request > %zu bytes) : %zu (unbounded: %zu) ===\n",
                kAllocSuspicious, greediest.size(), capHits);
    for (std::size_t i = 0; i < greediest.size() && i < 20; ++i) {
        const auto &f = greediest[i];
        std::printf("  %slargest=%-14zu total=%-14zu pid=%-4u %-34s %s\n", f.capHit ? "UNBOUNDED " : "", f.largestAlloc,
                    f.totalAlloc, f.pid, f.packet.c_str(), f.caseName.c_str());
    }
    if (greediest.size() > 20) {
        std::printf("  ... and %zu more\n", greediest.size() - 20);
    }
    std::printf("\n");

    // Corpus inventory, so the coverage claim can be judged rather than taken on trust.
    std::printf("=== corpus inventory ===\n");
    for (const auto &c : corpus) {
        std::printf("  %-26s %s\n", c.name.c_str(), hexPreview(c.data).c_str());
    }
    std::printf("\n");

    const bool failed = !criticals.empty() || capHits != 0;
    std::printf("%s\n", failed ? "RESULT: FAIL" : "RESULT: PASS");
    return failed ? 1 : 0;
}
