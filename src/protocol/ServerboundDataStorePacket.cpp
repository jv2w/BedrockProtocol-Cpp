/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerboundDataStorePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerboundDataStorePacket.h"

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

ServerboundDataStorePacket ServerboundDataStorePacket::create(std::optional<types::ddui::DataStoreUpdate> update)
{
    ServerboundDataStorePacket result;
    result.update = std::move(update);
    return result;
}

void ServerboundDataStorePacket::decodePayload(encoding::ByteBufferReader &in)
{
    update = types::ddui::DataStoreUpdate::read(in);

}

void ServerboundDataStorePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    update.value().write(out);

}

bool ServerboundDataStorePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerboundDataStore(*this);
}

}  // namespace bedrock_protocol
