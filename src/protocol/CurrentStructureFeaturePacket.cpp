/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CurrentStructureFeaturePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CurrentStructureFeaturePacket.h"

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

CurrentStructureFeaturePacket CurrentStructureFeaturePacket::create(std::string currentStructureFeature)
{
    CurrentStructureFeaturePacket result;
    result.currentStructureFeature = std::move(currentStructureFeature);
    return result;
}

void CurrentStructureFeaturePacket::decodePayload(encoding::ByteBufferReader &in)
{
    currentStructureFeature = serializer::CommonTypes::getString(in);

}

void CurrentStructureFeaturePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, currentStructureFeature);

}

bool CurrentStructureFeaturePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCurrentStructureFeature(*this);
}

}  // namespace bedrock_protocol
