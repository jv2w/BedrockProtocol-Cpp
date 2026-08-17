/*
 * This file is part of BedrockProtocol-Cpp.
 * This file has no PHP counterpart: the PHP original targets one protocol version at a time, and
 * this one has to hold two layouts of the same protocol number at once.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace bedrock_protocol::encoding {

/**
 * Which of protocol 2168's two wire layouts a buffer is written in, or is being read as.
 *
 * A protocol number used to pin one layout, so nothing but the bytes mattered. 1.26.44 changed
 * SetScorePacket's remove entry without moving the number, so 2168 is no longer one format: 1.26.40
 * through 1.26.43 speak one and 1.26.44 the other, and a client on either side of that line still
 * completes the handshake against a server on the other. Nothing on the wire tells the two apart -
 * an absent objective name is the same single byte under both - so the layout has to be KNOWN, from
 * the version each end reports, and never inferred from the bytes.
 *
 * Ordered oldest first, so a layout introduced in one version is selected with >= and stays selected
 * for every version after it.
 */
enum class ProtocolDialect : std::uint8_t {
    /** Minecraft 1.26.40 through 1.26.43. */
    V1_26_40 = 0,
    /** Minecraft 1.26.44 onwards, for as long as the protocol number stays 2168. */
    V1_26_44 = 1,
};

/** The layout this library reads and writes when it is not told otherwise. Tracks ProtocolInfo. */
inline constexpr ProtocolDialect CURRENT_DIALECT = ProtocolDialect::V1_26_44;

/**
 * Maps a Minecraft version string onto the layout that version speaks.
 *
 * Takes what the two runtime sources actually produce, and they do not agree about the leading 1.
 * Endstone's Server::getMinecraftVersion() gives the marketing form the game itself shows, two
 * components with the release number first ("26.40"). Player::getGameVersion() carries the version
 * out of the login, which is the full form and usually four components ("1.26.44.3"). Either may
 * carry a leading 'v'. The fourth component is a hotfix number and never changes the wire, so it is
 * deliberately not read.
 *
 * Anything else - an empty string from a client that reported no version, a version older or newer
 * than the range this library covers, a shape this cannot parse - is nullopt, which callers must
 * read as "do not guess" rather than as a default. Guessing here rewrites live traffic against a
 * layout nobody asked for.
 *
 * Assumes the connection negotiated protocol 2168; the caller knows that and this function cannot.
 */
[[nodiscard]] inline std::optional<ProtocolDialect> dialectFromGameVersion(std::string_view version)
{
    if (!version.empty() && (version.front() == 'v' || version.front() == 'V')) {
        version.remove_prefix(1);
    }

    unsigned int component[3] = {0, 0, 0};
    std::size_t index = 0;
    bool digitSeen = false;
    for (const char c : version) {
        if (c >= '0' && c <= '9') {
            if (component[index] > 100000U) {
                return std::nullopt;  // wider than any version number; not one
            }
            component[index] = component[index] * 10U + static_cast<unsigned int>(c - '0');
            digitSeen = true;
            continue;
        }
        if (c != '.' || !digitSeen) {
            return std::nullopt;
        }
        if (++index == 3) {
            break;  // a fourth component may follow, and is deliberately not read
        }
        digitSeen = false;
    }
    if (!digitSeen || index == 0) {
        return std::nullopt;  // a bare number is not a version
    }

    // `index` is how many dots were consumed, which is what tells the two spellings apart: one dot is
    // the marketing form ("26.40"), two or more is the full form ("1.26.44.3") and its leading
    // component has to be the 1 that form always carries.
    if (index >= 2 && component[0] != 1) {
        return std::nullopt;
    }
    const unsigned int release = index >= 2 ? component[1] : component[0];
    const unsigned int update = index >= 2 ? component[2] : component[1];

    if (release != 26) {
        return std::nullopt;
    }
    if (update >= 44) {
        return ProtocolDialect::V1_26_44;
    }
    if (update >= 40) {
        return ProtocolDialect::V1_26_40;
    }
    return std::nullopt;
}

}  // namespace bedrock_protocol::encoding
