/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/DataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/DataPacket.h"

#include <string>

#include "bedrock_protocol/encoding/DataDecodeException.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol {

using encoding::DataDecodeException;
using encoding::VarInt;

void DataPacket::decode(encoding::ByteBufferReader &in)
{
    try {
        decodeHeader(in);
        decodePayload(in);
    }
    catch (const DataDecodeException &e) {
        throw PacketDecodeException::wrap(e, getName());
    }
    catch (const PacketDecodeException &e) {
        throw PacketDecodeException::wrap(e, getName());
    }
}

void DataPacket::decodeHeader(encoding::ByteBufferReader &in)
{
    const auto header = VarInt::readUnsignedInt(in);
    const auto pid = header & PID_MASK;
    if (pid != networkId()) {
        // TODO: this means a logical error in the code, but how to prevent it from happening?
        throw PacketDecodeException("Expected " + std::to_string(networkId()) + " for packet ID, got " +
                                    std::to_string(pid));
    }
    senderSubId = static_cast<std::uint8_t>((header >> SENDER_SUBCLIENT_ID_SHIFT) & SUBCLIENT_ID_MASK);
    recipientSubId = static_cast<std::uint8_t>((header >> RECIPIENT_SUBCLIENT_ID_SHIFT) & SUBCLIENT_ID_MASK);
}

void DataPacket::encode(encoding::ByteBufferWriter &out) const
{
    encodeHeader(out);
    encodePayload(out);
}

void DataPacket::encodeHeader(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, networkId() | (static_cast<std::uint32_t>(senderSubId) << SENDER_SUBCLIENT_ID_SHIFT) |
                                      (static_cast<std::uint32_t>(recipientSubId) << RECIPIENT_SUBCLIENT_ID_SHIFT));
}

void DataPacket::decodeBody(encoding::ByteBufferReader &in)
{
    try {
        decodePayload(in);
    }
    catch (const DataDecodeException &e) {
        throw PacketDecodeException::wrap(e, getName());
    }
    catch (const PacketDecodeException &e) {
        throw PacketDecodeException::wrap(e, getName());
    }
}

void DataPacket::encodeBody(encoding::ByteBufferWriter &out) const
{
    encodePayload(out);
}

}  // namespace bedrock_protocol
