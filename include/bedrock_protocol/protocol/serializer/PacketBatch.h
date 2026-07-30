/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/serializer/PacketBatch.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/PacketPool.h"

namespace bedrock_protocol::serializer {

/**
 * Batch framing: a batch is simply a sequence of [unsigned varint length][payload] pairs.
 *
 * @note DELIBERATE DEVIATION: the PHP original returns a \Generator from decodeRaw()/decodePackets()
 * so that callers can process packets lazily. C++ has no coroutine-free equivalent here, so this
 * port returns a std::vector containing all of the decoded entries instead. The wire format and the
 * "keep going while there is unread data" loop semantics are unchanged; only the laziness is lost.
 */
class PacketBatch {
public:
    /**
     * @throws PacketDecodeException
     */
    static std::vector<std::string> decodeRaw(encoding::ByteBufferReader &in);

    /**
     * @param packets
     */
    static void encodeRaw(encoding::ByteBufferWriter &out, const std::vector<std::string> &packets);

    /**
     * @throws PacketDecodeException
     */
    static std::vector<std::unique_ptr<DataPacket>> decodePackets(encoding::ByteBufferReader &in,
                                                                  const PacketPool &packetPool);

    /**
     * @param packets
     */
    static void encodePackets(encoding::ByteBufferWriter &out, const std::vector<const Packet *> &packets);

private:
    PacketBatch() = default; //NOOP
};

}  // namespace bedrock_protocol::serializer
