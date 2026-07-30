/*
 * This file is part of BedrockProtocol for Endstone.
 */

#include "bedrock_protocol/verify/Report.h"

#include <algorithm>
#include <cstdio>

namespace bedrock_protocol::verify {

namespace {

constexpr char HEX_DIGITS[] = "0123456789abcdef";

void appendHexByte(std::string &out, unsigned char value)
{
    out.push_back(HEX_DIGITS[value >> 4]);
    out.push_back(HEX_DIGITS[value & 0x0f]);
}

/** Thousands separators: these counters run to five and six figures and are meant to be skimmed. */
std::string grouped(std::size_t value)
{
    std::string digits = std::to_string(value);
    std::string out;
    const std::size_t lead = digits.size() % 3 == 0 ? 3 : digits.size() % 3;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i == lead && i != 0) {
            out.push_back(',');
        }
        else if (i > lead && (i - lead) % 3 == 0) {
            out.push_back(',');
        }
        out.push_back(digits[i]);
    }
    return out;
}

}  // namespace

std::string_view toString(Stage stage)
{
    switch (stage) {
    case Stage::Fill:
        return "fill";
    case Stage::Encode:
        return "encode";
    case Stage::AntiTautology:
        return "anti-tautology";
    case Stage::Decode:
        return "decode";
    case Stage::Drain:
        return "drain";
    case Stage::ReEncode:
        return "re-encode";
    case Stage::Idempotence:
        return "idempotence";
    case Stage::HeaderPath:
        return "header path";
    case Stage::Truncation:
        return "truncation fuzz";
    case Stage::Corruption:
        return "corruption fuzz";
    case Stage::Passed:
        return "passed";
    }
    return "?";
}

std::size_t firstDifference(std::string_view a, std::string_view b)
{
    const std::size_t shared = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < shared; ++i) {
        if (a[i] != b[i]) {
            return i;
        }
    }
    return shared;
}

std::string hexWindow(std::string_view bytes, std::size_t offset, std::size_t radius)
{
    if (bytes.empty()) {
        return "<empty>";
    }
    const std::size_t begin = offset > radius ? offset - radius : 0;
    const std::size_t end = std::min(bytes.size(), offset + radius + 1);

    std::string out;
    if (begin > 0) {
        out.append("... ");
    }
    for (std::size_t i = begin; i < end; ++i) {
        if (i == offset) {
            out.push_back('[');
        }
        appendHexByte(out, static_cast<unsigned char>(bytes[i]));
        if (i == offset) {
            out.push_back(']');
        }
        if (i + 1 < end) {
            out.push_back(' ');
        }
    }
    if (end < bytes.size()) {
        out.append(" ...");
    }
    return out;
}

std::string explainMismatch(std::string_view expected, std::string_view actual, std::size_t offset)
{
    if (expected.size() != actual.size()) {
        const std::size_t delta = expected.size() > actual.size() ? expected.size() - actual.size()
                                                                  : actual.size() - expected.size();
        // A varint that should have been 32-bit written as 64-bit (or a float written as a double)
        // shifts the total length by a small, characteristic amount.
        if (delta <= 5) {
            return "length differs by " + std::to_string(delta) +
                   " byte(s), which is the size of one varint/fixed-width step: suspect an integer "
                   "width or float/double mix-up at this field";
        }
        return "length differs by " + std::to_string(delta) +
               " bytes: suspect a field that is written but not read, or an omitted collection";
    }

    // Endianness: the same bytes in the opposite order over a natural scalar width.
    for (const std::size_t width : {2u, 4u, 8u}) {
        if (offset + width > expected.size()) {
            continue;
        }
        const auto left = expected.substr(offset, width);
        const auto right = actual.substr(offset, width);
        if (left == right) {
            continue;
        }
        if (std::equal(left.begin(), left.end(), right.rbegin(), right.rend())) {
            return "the " + std::to_string(width) +
                   "-byte window is byte-reversed: suspect an endianness error (LE vs BE) on this field";
        }
    }

    // A field written with the wrong length shifts everything after it by a constant.
    for (std::size_t shift = 1; shift <= 4; ++shift) {
        if (offset + shift >= expected.size()) {
            break;
        }
        const auto tailExpected = expected.substr(offset + shift);
        const auto tailActual = actual.substr(offset, tailExpected.size());
        if (tailExpected.size() >= 8 && tailExpected == tailActual) {
            return "everything after this point is shifted by " + std::to_string(shift) +
                   " byte(s): suspect a missing or extra field right here";
        }
    }

    return "same length, first divergence at this byte: suspect a wrong value, a swapped field pair, "
           "or a discriminator that selects a different branch on decode";
}

int Report::passedCount() const
{
    return static_cast<int>(std::count_if(results.begin(), results.end(),
                                          [](const PacketResult &r) { return r.passed; }));
}

int Report::fieldsCovered() const
{
    int total = 0;
    for (const auto &result : results) {
        total += result.declaredFields;
    }
    return total;
}

std::size_t Report::truncationPrefixes() const
{
    std::size_t total = 0;
    for (const auto &result : results) {
        total += result.truncationPrefixes;
    }
    return total;
}

std::size_t Report::corruptionPasses() const
{
    std::size_t total = 0;
    for (const auto &result : results) {
        total += result.corruptionPasses;
    }
    return total;
}

std::string Report::toPlainText() const
{
    std::string out;
    char line[512];

    std::snprintf(line, sizeof(line), "BedrockProtocol deep round-trip   seed 0x%llX\n\n",
                  static_cast<unsigned long long>(seed));
    out.append(line);

    std::snprintf(line, sizeof(line), "  packets in suite      %4d        fields covered      %6d\n", total(),
                  fieldsCovered());
    out.append(line);
    std::snprintf(line, sizeof(line), "  round-tripped         %4d        duplicate values    %6d\n",
                  passedCount(), duplicateValues);
    out.append(line);

    if (fuzzEnabled) {
        std::snprintf(line, sizeof(line), "  truncation prefixes %s        corruption passes  %s\n",
                      grouped(truncationPrefixes()).c_str(), grouped(corruptionPasses()).c_str());
        out.append(line);
    }
    else {
        out.append("  fuzzing               skipped (hardware-fault translation unavailable)\n");
    }

    int vacuous = 0;
    int notRun = 0;
    for (const auto &result : results) {
        if (!result.antiTautologyRun) {
            ++notRun;
        }
        else if (!result.defaultEncodable) {
            ++vacuous;
        }
    }
    if (notRun > 0) {
        out.append("\n  anti-tautology skipped for the whole run: it must encode a default-constructed\n"
                   "  packet, which faults for a few of them, and this build cannot catch a fault.\n");
    }
    if (vacuous > 0) {
        std::snprintf(line, sizeof(line),
                      "\n  %d packet(s) cannot encode from their default state, so the anti-tautology\n"
                      "  comparison was vacuous for them (this mirrors PHP and is not a defect).\n",
                      vacuous);
        out.append(line);
    }

    const int failures = total() - passedCount();
    std::snprintf(line, sizeof(line), "\nFAILURES (%d)\n", failures);
    out.append(line);

    for (const auto &result : results) {
        if (result.passed) {
            continue;
        }
        std::snprintf(line, sizeof(line), "\n  %s (0x%02x)\n", result.name.c_str(), result.pid);
        out.append(line);
        out.append("    stage        ").append(toString(result.failedAt)).append("\n");
        if (!result.detail.empty()) {
            out.append("    detail       ").append(result.detail).append("\n");
        }
        if (result.hasDiff) {
            std::snprintf(line, sizeof(line), "    first diff   offset %zu (expected %zu bytes, got %zu)\n",
                          result.diffOffset, result.expectedSize, result.actualSize);
            out.append(line);
            out.append("    expected     ").append(result.expectedWindow).append("\n");
            out.append("    actual       ").append(result.actualWindow).append("\n");
            out.append("    hint         ").append(result.hint).append("\n");
        }
        std::snprintf(line, sizeof(line),
                      "    repro        run_deep_roundtrip.ps1 -Only %s -Seed 0x%llX\n", result.name.c_str(),
                      static_cast<unsigned long long>(seed));
        out.append(line);
    }

    if (failures == 0) {
        out.append("  none\n");
    }
    return out;
}

std::vector<std::string> Report::toChatLines() const
{
    std::vector<std::string> lines;
    char buffer[256];

    const int failures = total() - passedCount();
    std::snprintf(buffer, sizeof(buffer), "§7정밀 검사 §f%d§7개 패킷 · §f%d§7개 필드 · 시드 §f0x%llX", total(),
                  fieldsCovered(), static_cast<unsigned long long>(seed));
    lines.emplace_back(buffer);

    if (failures == 0) {
        std::snprintf(buffer, sizeof(buffer), "§a전부 통과 §7- 왕복 %d/%d, 중복값 %d", passedCount(), total(),
                      duplicateValues);
        lines.emplace_back(buffer);
        if (fuzzEnabled) {
            std::snprintf(buffer, sizeof(buffer), "§7퍼징: 절단 §f%s§7 · 변조 §f%s§7 · 폴트 §a0",
                          grouped(truncationPrefixes()).c_str(), grouped(corruptionPasses()).c_str());
            lines.emplace_back(buffer);
        }
        return lines;
    }

    std::snprintf(buffer, sizeof(buffer), "§c실패 %d개 §7(통과 %d/%d)", failures, passedCount(), total());
    lines.emplace_back(buffer);

    // Failures matter more than the summary, so they get the remaining budget.
    int shown = 0;
    for (const auto &result : results) {
        if (result.passed) {
            continue;
        }
        if (shown++ >= 12) {
            std::snprintf(buffer, sizeof(buffer), "§7... 그 외 %d개. 전체는 콘솔 로그 참고",
                          failures - shown + 1);
            lines.emplace_back(buffer);
            break;
        }
        std::snprintf(buffer, sizeof(buffer), "§c %s §7- %.*s", result.name.c_str(),
                      static_cast<int>(toString(result.failedAt).size()), toString(result.failedAt).data());
        lines.emplace_back(buffer);
    }
    return lines;
}

}  // namespace bedrock_protocol::verify
