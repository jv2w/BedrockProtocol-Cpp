/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetTimePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetTimePacket.h"

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

SetTimePacket SetTimePacket::create(std::int32_t time)
{
    SetTimePacket result;
    // Was missing entirely, so create() returned a default packet and the time was silently dropped.
    // PHP masks with 0xffffffff to keep the value inside an int32; that is implicit here.
    result.time = time;
    return result;
}

void SetTimePacket::decodePayload(encoding::ByteBufferReader &in)
{
    time = encoding::VarInt::readSignedInt(in);

}

void SetTimePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, time);

}

bool SetTimePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetTime(*this);
}

}  // namespace bedrock_protocol
