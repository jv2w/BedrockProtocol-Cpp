/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/EduUriResourcePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/EduUriResourcePacket.h"

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

EduUriResourcePacket EduUriResourcePacket::create(types::EducationUriResource resource)
{
    EduUriResourcePacket result;
    result.resource = std::move(resource);
    return result;
}

void EduUriResourcePacket::decodePayload(encoding::ByteBufferReader &in)
{
    resource = types::EducationUriResource::read(in);

}

void EduUriResourcePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    resource->write(out);

}

bool EduUriResourcePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleEduUriResource(*this);
}

}  // namespace bedrock_protocol
