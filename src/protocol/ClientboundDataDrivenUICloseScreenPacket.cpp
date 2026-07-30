/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundDataDrivenUICloseScreenPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundDataDrivenUICloseScreenPacket.h"

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

ClientboundDataDrivenUICloseScreenPacket ClientboundDataDrivenUICloseScreenPacket::create(std::optional<std::uint32_t> formId)
{
    ClientboundDataDrivenUICloseScreenPacket result;
    result.formId = std::move(formId);
    return result;
}

void ClientboundDataDrivenUICloseScreenPacket::decodePayload(encoding::ByteBufferReader &in)
{
    formId = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readUnsignedInt(reader); });

}

void ClientboundDataDrivenUICloseScreenPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(out, formId, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeUnsignedInt(writer, value); });

}

bool ClientboundDataDrivenUICloseScreenPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundDataDrivenUICloseScreen(*this);
}

}  // namespace bedrock_protocol
