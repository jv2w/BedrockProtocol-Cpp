/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Writes every selected packet, fully populated, as the bytes it puts on the wire.
 *
 * tools/check_php_parity.py proves the two implementations are spelled the same way. That is a
 * comparison of source text, and the repeated-key case showed what it cannot see: identical code
 * whose behaviour differs because the two languages disagree about what it means. Only running both
 * and comparing bytes closes that.
 *
 * The output feeds tools/php_wire_parity.php, which hands each line to the PHP original and asks it
 * to decode and re-encode. PHP returning the same bytes means it read every field the way this port
 * wrote it - which is the claim the whole port rests on.
 *
 * The bytes are the full encode() output, header included, so PHP can route each one through
 * PacketPool exactly as it would a packet off the network.
 */

#include <cstdio>
#include <memory>
#include <string>

#include "TestHarness.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/verify/Filler.h"
#include "bedrock_protocol/verify/ValueWell.h"

using namespace bedrock_protocol;

int main()
{
    verify::ValueWell well(0x5DEECE66DULL);

    std::printf("# BedrockProtocol-Cpp wire fixtures - protocol %d (%.*s), seed 0x5DEECE66D\n",
                ProtocolInfo::CURRENT_PROTOCOL,
                static_cast<int>(ProtocolInfo::MINECRAFT_VERSION_NETWORK.size()),
                ProtocolInfo::MINECRAFT_VERSION_NETWORK.data());
    std::printf("# <name> <pid> <hex of encode(), header included>\n");

    int written = 0;
    int skipped = 0;
    for (const auto &entry : verify::getSelection()) {
        well.beginPacket(entry.pid);
        verify::FillContext context{well};

        std::unique_ptr<DataPacket> packet;
        try {
            packet = entry.fill(context);
        }
        catch (const std::exception &e) {
            std::fprintf(stderr, "skip %s: filler threw: %s\n", std::string(entry.name).c_str(), e.what());
            ++skipped;
            continue;
        }

        encoding::ByteBufferWriter out;
        try {
            packet->encode(out);
        }
        catch (const std::exception &e) {
            std::fprintf(stderr, "skip %s: encode threw: %s\n", std::string(entry.name).c_str(), e.what());
            ++skipped;
            continue;
        }

        std::printf("%.*s %u %s\n", static_cast<int>(entry.name.size()), entry.name.data(), entry.pid,
                    test::toHex(out.getData()).c_str());
        ++written;
    }

    std::fprintf(stderr, "%d fixtures written, %d skipped\n", written, skipped);
    return skipped == 0 ? 0 : 1;
}
