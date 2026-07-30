/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SyncWorldClocksPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SyncWorldClocksPacket.h"

#include <stdexcept>
#include <memory>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

SyncWorldClocksPacket SyncWorldClocksPacket::create(std::unique_ptr<types::SyncWorldClocksPayload> payload)
{
    SyncWorldClocksPacket result;
    result.payload = std::move(payload);
    return result;
}

void SyncWorldClocksPacket::decodePayload(encoding::ByteBufferReader &in)
{
    switch (static_cast<std::int32_t>(encoding::VarInt::readUnsignedInt(in))) {
        case types::SyncWorldClocksSyncState::ID:
            payload = std::make_unique<types::SyncWorldClocksSyncState>(types::SyncWorldClocksSyncState::read(in));
            break;
        case types::SyncWorldClocksInitializeRegistry::ID:
            payload = std::make_unique<types::SyncWorldClocksInitializeRegistry>(
                types::SyncWorldClocksInitializeRegistry::read(in));
            break;
        case types::SyncWorldClocksAddTimeMarker::ID:
            payload =
                std::make_unique<types::SyncWorldClocksAddTimeMarker>(types::SyncWorldClocksAddTimeMarker::read(in));
            break;
        case types::SyncWorldClocksRemoveTimeMarker::ID:
            payload = std::make_unique<types::SyncWorldClocksRemoveTimeMarker>(
                types::SyncWorldClocksRemoveTimeMarker::read(in));
            break;
        default:
            throw PacketDecodeException("Unknown SyncWorldClocks type");
    }
}

void SyncWorldClocksPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, payload->getTypeId());
    payload->write(out);

}

bool SyncWorldClocksPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSyncWorldClocks(*this);
}

}  // namespace bedrock_protocol
