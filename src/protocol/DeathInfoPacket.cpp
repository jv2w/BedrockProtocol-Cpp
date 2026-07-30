/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/DeathInfoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/DeathInfoPacket.h"

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

DeathInfoPacket DeathInfoPacket::create(std::string messageTranslationKey, std::vector<std::string> messageParameters)
{
    DeathInfoPacket result;
    result.messageTranslationKey = std::move(messageTranslationKey);
    result.messageParameters = std::move(messageParameters);
    return result;
}

void DeathInfoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    messageTranslationKey = serializer::CommonTypes::getString(in);

    messageParameters.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        messageParameters.push_back(serializer::CommonTypes::getString(in));
    }

}

void DeathInfoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, messageTranslationKey);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(messageParameters.size()));
    for (const auto &parameter : messageParameters) {
        serializer::CommonTypes::putString(out, parameter);
    }

}

bool DeathInfoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleDeathInfo(*this);
}

}  // namespace bedrock_protocol
