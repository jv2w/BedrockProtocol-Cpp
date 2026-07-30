/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerSettingsResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerSettingsResponsePacket.h"

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

ServerSettingsResponsePacket ServerSettingsResponsePacket::create(std::uint32_t formId, std::string formData)
{
    ServerSettingsResponsePacket result;
    result.formId = formId;
    result.formData = std::move(formData);
    return result;
}

void ServerSettingsResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    formId = encoding::VarInt::readUnsignedInt(in);
    formData = serializer::CommonTypes::getString(in);

}

void ServerSettingsResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, formId);
    serializer::CommonTypes::putString(out, formData);

}

bool ServerSettingsResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerSettingsResponse(*this);
}

}  // namespace bedrock_protocol
