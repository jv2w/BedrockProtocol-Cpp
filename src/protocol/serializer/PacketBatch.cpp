/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/PacketBatch.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/serializer/PacketBatch.h"

#include <utility>

#include "bedrock_protocol/encoding/DataDecodeException.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::serializer {

using encoding::DataDecodeException;
using encoding::VarInt;

std::vector<std::string> PacketBatch::decodeRaw(encoding::ByteBufferReader &in)
{
    std::vector<std::string> result;

    int c = 0;
    while (in.getUnreadLength() > 0) {
        std::string_view buffer;
        try {
            const auto length = VarInt::readUnsignedInt(in);
            buffer = in.readByteArray(length);
        }
        catch (const DataDecodeException &e) {
            throw PacketDecodeException("Error decoding packet " + std::to_string(c) + " in batch: " + e.what());
        }
        result.emplace_back(buffer);
        c++;
    }

    return result;
}

void PacketBatch::encodeRaw(encoding::ByteBufferWriter &out, const std::vector<std::string> &packets)
{
    for (const auto &packet : packets) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(packet.size()));
        out.writeByteArray(packet);
    }
}

std::vector<std::unique_ptr<DataPacket>> PacketBatch::decodePackets(encoding::ByteBufferReader &in,
                                                                    const PacketPool &packetPool)
{
    std::vector<std::unique_ptr<DataPacket>> result;

    int c = 0;
    for (const auto &packetBuffer : decodeRaw(in)) {
        auto packet = packetPool.getPacket(packetBuffer);
        if (packet != nullptr) {
            try {
                //TODO: this could use a view with a start and end offset to avoid extra string allocations
                //currently ByteBufferReader doesn't support this
                encoding::ByteBufferReader reader(packetBuffer);
                packet->decode(reader);
            }
            catch (const PacketDecodeException &e) {
                throw PacketDecodeException("Error decoding packet " + std::to_string(c) + " in batch: " + e.what());
            }
            result.push_back(std::move(packet));
        }
        else {
            throw PacketDecodeException("Unknown packet " + std::to_string(c) + " in batch");
        }
        c++;
    }

    return result;
}

void PacketBatch::encodePackets(encoding::ByteBufferWriter &out, const std::vector<const Packet *> &packets)
{
    for (const auto *packet : packets) {
        encoding::ByteBufferWriter serializer;
        packet->encode(serializer);
        //this may require a copy, so don't call it twice
        const auto &packetBuffer = serializer.getData();
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(packetBuffer.size()));
        out.writeByteArray(packetBuffer);
    }
}

}  // namespace bedrock_protocol::serializer
