/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundAttributeLayerSyncPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundAttributeLayerSyncPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

ClientboundAttributeLayerSyncPacket ClientboundAttributeLayerSyncPacket::create(std::unique_ptr<types::AttributeLayerSyncPayload> payload)
{
    ClientboundAttributeLayerSyncPacket result;
    result.payload = std::move(payload);
    return result;
}

void ClientboundAttributeLayerSyncPacket::decodePayload(encoding::ByteBufferReader &in)
{
    switch (encoding::VarInt::readUnsignedInt(in)) {
        case types::AttributeUpdateLayers::ID:
            payload = std::make_unique<types::AttributeUpdateLayers>(types::AttributeUpdateLayers::read(in));
            break;
        case types::AttributeUpdateLayerSettings::ID:
            payload = std::make_unique<types::AttributeUpdateLayerSettings>(types::AttributeUpdateLayerSettings::read(in));
            break;
        case types::AttributesUpdateEnvironment::ID:
            payload = std::make_unique<types::AttributesUpdateEnvironment>(types::AttributesUpdateEnvironment::read(in));
            break;
        case types::AttributesRemoveEnvironment::ID:
            payload = std::make_unique<types::AttributesRemoveEnvironment>(types::AttributesRemoveEnvironment::read(in));
            break;
        default:
            throw PacketDecodeException("Unknown ClientboundAttributeLayerSync type");
    }

}

void ClientboundAttributeLayerSyncPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(payload->getTypeId()));
    payload->write(out);

}

bool ClientboundAttributeLayerSyncPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundAttributeLayerSync(*this);
}

}  // namespace bedrock_protocol
