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

#pragma once

#include <cstdint>

#include "bedrock_protocol/protocol/Packet.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol {

/** Base class of every concrete packet, providing the generic header handling. */
class DataPacket : public Packet {
public:
    static constexpr std::uint32_t NETWORK_ID = 0;

    static constexpr std::uint32_t PID_MASK = 0x3ff;  // 10 bits

    std::uint8_t senderSubId = 0;
    std::uint8_t recipientSubId = 0;

    [[nodiscard]] std::uint32_t pid() const final { return networkId(); }

    /**
     * Returns this packet's network ID.
     *
     * PHP resolves `static::NETWORK_ID` through late static binding; C++ has no equivalent, so each
     * concrete packet overrides this to return its own NETWORK_ID constant.
     */
    [[nodiscard]] virtual std::uint32_t networkId() const = 0;

    [[nodiscard]] bool canBeSentBeforeLogin() const override { return false; }

    /**
     * @throws PacketDecodeException
     */
    void decode(encoding::ByteBufferReader &in) final;

    void encode(encoding::ByteBufferWriter &out) const final;

    /**
     * Decodes the packet body only, assuming the generic header has already been consumed.
     *
     * This has no PHP counterpart: Endstone's packet events deliver the payload with the header
     * already stripped, so the bridge needs an entry point that skips decodeHeader().
     *
     * @throws PacketDecodeException
     */
    void decodeBody(encoding::ByteBufferReader &in);

    /**
     * Encodes the packet body only, without the generic header.
     *
     * Counterpart of decodeBody(); see the note there.
     */
    void encodeBody(encoding::ByteBufferWriter &out) const;

protected:
    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    void decodeHeader(encoding::ByteBufferReader &in);

    void encodeHeader(encoding::ByteBufferWriter &out) const;

    /**
     * Decodes the packet body, without the packet ID or other generic header fields.
     *
     * @throws PacketDecodeException
     * @throws DataDecodeException
     */
    virtual void decodePayload(encoding::ByteBufferReader &in) = 0;

    /**
     * Encodes the packet body, without the packet ID or other generic header fields.
     */
    virtual void encodePayload(encoding::ByteBufferWriter &out) const = 0;

private:
    static constexpr std::uint32_t SUBCLIENT_ID_MASK = 0x03;  // 2 bits
    static constexpr int SENDER_SUBCLIENT_ID_SHIFT = 10;
    static constexpr int RECIPIENT_SUBCLIENT_ID_SHIFT = 12;
};

}  // namespace bedrock_protocol
