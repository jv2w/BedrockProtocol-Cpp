/*
 * This file is part of BedrockProtocol-Cpp.
 * Not present in the PHP original; models gophertunnel's protocol.TeleportData
 * (minecraft/protocol/player.go:179-190).
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/TeleportData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::LE;

TeleportData TeleportData::read(encoding::ByteBufferReader &in)
{
    // player.go:188-189 - both are fixed 4-byte little-endian ints.
    const auto teleportCause = LE::readSignedInt(in);
    const auto teleportItem = LE::readSignedInt(in);

    return {teleportCause, teleportItem};
}

void TeleportData::write(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedInt(out, teleportCause);
    LE::writeSignedInt(out, teleportItem);
}

}  // namespace bedrock_protocol::types
