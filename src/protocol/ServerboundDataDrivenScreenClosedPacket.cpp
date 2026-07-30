/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ServerboundDataDrivenScreenClosedPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerboundDataDrivenScreenClosedPacket.h"

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

ServerboundDataDrivenScreenClosedPacket ServerboundDataDrivenScreenClosedPacket::create(std::uint32_t formId, std::string closeReason)
{
    ServerboundDataDrivenScreenClosedPacket result;
    result.formId = formId;
    result.closeReason = std::move(closeReason);
    return result;
}

void ServerboundDataDrivenScreenClosedPacket::decodePayload(encoding::ByteBufferReader &in)
{
    formId = encoding::LE::readUnsignedInt(in);
    closeReason = serializer::CommonTypes::getString(in);

}

void ServerboundDataDrivenScreenClosedPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedInt(out, formId);
    serializer::CommonTypes::putString(out, closeReason);

}

bool ServerboundDataDrivenScreenClosedPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerboundDataDrivenScreenClosed(*this);
}

}  // namespace bedrock_protocol
