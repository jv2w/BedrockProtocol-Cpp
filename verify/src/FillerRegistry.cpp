/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Builds the dispatch table from PacketSelection.inc.
 *
 * Note what the two passes below buy: adding a line to the .inc without writing the matching
 * fillXxxPacket produces a LINK ERROR. Completeness of the suite is enforced by the toolchain, not
 * by anyone remembering to keep two lists in step.
 */

#include "bedrock_protocol/verify/Filler.h"

#include <algorithm>
#include <array>
#include <cctype>

#include "bedrock_protocol/protocol/AllPackets.h"

namespace bedrock_protocol::verify {

std::string_view toString(Tier tier)
{
    switch (tier) {
    case Tier::Core:
        return "Core";
    case Tier::Common:
        return "Common";
    case Tier::Rare:
        return "Rare";
    }
    return "?";
}

// Pass 1: declare every filler the list names.
//
// Deliberately at namespace scope rather than inside the anonymous namespace below. That is what
// makes the enforcement real: with external linkage, a packet listed in the .inc without a matching
// filler is an unresolved symbol at link time. Given internal linkage it would merely be a warning
// and the table would end up holding a pointer to nothing.
#define BP_SELECTED_PACKET(Type, TierName, Reasons, Fields, SampleEvery, Note) \
    std::unique_ptr<DataPacket> fill##Type(FillContext &);
#include "bedrock_protocol/verify/PacketSelection.inc"
#undef BP_SELECTED_PACKET

namespace {

// Pass 2: the table itself.
#define BP_SELECTED_PACKET(Type, TierName, Reasons, Fields, SampleEvery, Note)                    \
    SelectedPacket{Type::NETWORK_ID, #Type, Tier::TierName, Reasons, Fields, SampleEvery, Note,   \
                   &fill##Type},
const SelectedPacket TABLE[] = {
#include "bedrock_protocol/verify/PacketSelection.inc"
};
#undef BP_SELECTED_PACKET

char lowerAscii(char c)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

}  // namespace

std::span<const SelectedPacket> getSelection()
{
    return {TABLE, std::size(TABLE)};
}

const SelectedPacket *findSelected(std::uint32_t pid)
{
    for (const auto &entry : TABLE) {
        if (entry.pid == pid) {
            return &entry;
        }
    }
    return nullptr;
}

const SelectedPacket *findSelectedByName(std::string_view name)
{
    const auto equalsIgnoringCase = [](std::string_view a, std::string_view b) {
        return a.size() == b.size() &&
               std::equal(a.begin(), a.end(), b.begin(),
                          [](char x, char y) { return lowerAscii(x) == lowerAscii(y); });
    };

    for (const auto &entry : TABLE) {
        if (equalsIgnoringCase(entry.name, name)) {
            return &entry;
        }
    }
    // "MovePlayer" should find MovePlayerPacket: the suffix is noise when typing a command.
    for (const auto &entry : TABLE) {
        if (entry.name.size() > 6 && equalsIgnoringCase(entry.name.substr(0, entry.name.size() - 6), name)) {
            return &entry;
        }
    }
    return nullptr;
}

}  // namespace bedrock_protocol::verify
