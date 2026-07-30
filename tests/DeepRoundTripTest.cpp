/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * The authoritative gate for the deep round-trip suite.
 *
 * PacketRoundTripTest covers all 229 packets shallowly, from their default-constructed state.
 * This covers the selected packets deeply, with every field populated. They answer different
 * questions and both are kept:
 *
 *   PacketRoundTripTest  "can every packet be constructed, encoded and decoded at all?"
 *   DeepRoundTripTest    "is every FIELD written and read correctly, and can the decoder be broken?"
 *
 * Usage:  DeepRoundTripTest [--only=PacketName] [--seed=0xHEX] [--no-fuzz]
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

#include "TestHarness.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/verify/DeepRoundTrip.h"

using namespace bedrock_protocol;

namespace {

bool consumeOption(std::string_view argument, std::string_view name, std::string_view &value)
{
    if (argument.size() <= name.size() || argument.substr(0, name.size()) != name) {
        return false;
    }
    value = argument.substr(name.size());
    return true;
}

}  // namespace

int main(int argc, char **argv)
{
    verify::Options options;

    for (int i = 1; i < argc; ++i) {
        const std::string_view argument(argv[i]);
        std::string_view value;
        if (consumeOption(argument, "--only=", value)) {
            options.only = std::string(value);
        }
        else if (consumeOption(argument, "--seed=", value)) {
            options.seed = std::strtoull(std::string(value).c_str(), nullptr, 0);
        }
        else if (argument == "--no-fuzz") {
            options.fuzz = false;
        }
        else {
            std::printf("unknown argument: %.*s\n", static_cast<int>(argument.size()), argument.data());
            return 2;
        }
    }

    std::printf("Protocol %d (%.*s)\n", ProtocolInfo::CURRENT_PROTOCOL,
                static_cast<int>(ProtocolInfo::MINECRAFT_VERSION_NETWORK.size()),
                ProtocolInfo::MINECRAFT_VERSION_NETWORK.data());

    const auto report = verify::runDeepRoundTrip(options);
    std::fputs(report.toPlainText().c_str(), stdout);

    if (!options.only.empty() && report.total() == 0) {
        std::printf("\n--only=%s matched no packet in the suite.\n", options.only.c_str());
        return 2;
    }

    if (options.fuzz && !report.faultTranslationAvailable) {
        // Not a failure - but the run is weaker than it looks, and saying so is the whole point of
        // having a gate. The MSVC build is the one that must be believed.
        std::printf("\nNOTE: hardware-fault translation is unavailable in this build, so the truncation\n"
                    "      and corruption stages were skipped. Run the MSVC build for the full gate.\n");
    }

    CHECK_TRUE(report.total() > 0);
    // A duplicate value means two fields of one packet could hold the same value, which would let a
    // swap between them go unnoticed. The suite's core assumption, so it is asserted, not logged.
    CHECK_EQ(report.duplicateValues, 0);
    CHECK_EQ(report.passedCount(), report.total());

    return test::summarise("DeepRoundTripTest");
}
