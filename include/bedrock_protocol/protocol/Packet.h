/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/Packet.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string_view>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

/** Common interface implemented by every packet in the protocol. */
class Packet {
public:
    virtual ~Packet() = default;

    /** Returns the network ID of this packet. */
    [[nodiscard]] virtual std::uint32_t pid() const = 0;

    /**
     * Returns the short class name of this packet, e.g. "TextPacket".
     *
     * PHP derives this from reflection; the C++ port stores it as a per-class constant.
     */
    [[nodiscard]] virtual std::string_view getName() const = 0;

    /** Returns whether this packet may legally be sent or received before the login sequence completes. */
    [[nodiscard]] virtual bool canBeSentBeforeLogin() const = 0;

    /**
     * Decodes the complete packet, including the generic header.
     *
     * @throws PacketDecodeException
     */
    virtual void decode(encoding::ByteBufferReader &in) = 0;

    /** Encodes the complete packet, including the generic header. */
    virtual void encode(encoding::ByteBufferWriter &out) const = 0;

    /**
     * Dispatches this packet to the matching handleXxx() method of the given handler.
     *
     * @return whatever the handler returned; false means the packet was not handled
     */
    virtual bool handle(PacketHandlerInterface &handler) = 0;
};

}  // namespace bedrock_protocol
