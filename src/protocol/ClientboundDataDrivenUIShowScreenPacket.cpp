/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundDataDrivenUIShowScreenPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundDataDrivenUIShowScreenPacket.h"

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

ClientboundDataDrivenUIShowScreenPacket ClientboundDataDrivenUIShowScreenPacket::create(std::string screenId, std::uint32_t formId, std::optional<std::uint32_t> dataInstanceId)
{
    ClientboundDataDrivenUIShowScreenPacket result;
    result.screenId = std::move(screenId);
    result.formId = formId;
    result.dataInstanceId = std::move(dataInstanceId);
    return result;
}

void ClientboundDataDrivenUIShowScreenPacket::decodePayload(encoding::ByteBufferReader &in)
{
    screenId = serializer::CommonTypes::getString(in);
    formId = encoding::LE::readUnsignedInt(in);
    dataInstanceId = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readUnsignedInt(reader); });

}

void ClientboundDataDrivenUIShowScreenPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, screenId);
    encoding::LE::writeUnsignedInt(out, formId);
    serializer::CommonTypes::writeOptional(out, dataInstanceId, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeUnsignedInt(writer, value); });

}

bool ClientboundDataDrivenUIShowScreenPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundDataDrivenUIShowScreen(*this);
}

}  // namespace bedrock_protocol
