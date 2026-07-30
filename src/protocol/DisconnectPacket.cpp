/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/DisconnectPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/DisconnectPacket.h"

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

DisconnectPacket DisconnectPacket::create(std::int32_t reason, std::optional<std::string> message, std::optional<std::string> filteredMessage)
{
    DisconnectPacket result;
    result.reason = reason;
    result.message = std::move(message);
    result.filteredMessage = std::move(filteredMessage);
    return result;
}

void DisconnectPacket::decodePayload(encoding::ByteBufferReader &in)
{
    reason = encoding::VarInt::readSignedInt(in);
    const auto type = encoding::VarInt::readUnsignedInt(in);
    message = type == 0 ? std::optional<std::string>(serializer::CommonTypes::getString(in)) : std::nullopt;
    filteredMessage = type == 0 ? std::optional<std::string>(serializer::CommonTypes::getString(in)) : std::nullopt;

}

void DisconnectPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, reason);
    const bool skipMessage = !message.has_value() && !filteredMessage.has_value();
    encoding::VarInt::writeUnsignedInt(out, skipMessage ? 1 : 0);
    if (!skipMessage) {
        serializer::CommonTypes::putString(out, message.value_or(""));
        serializer::CommonTypes::putString(out, filteredMessage.value_or(""));
    }

}

bool DisconnectPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleDisconnect(*this);
}

}  // namespace bedrock_protocol
