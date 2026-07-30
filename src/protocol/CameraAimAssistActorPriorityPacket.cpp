/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CameraAimAssistActorPriorityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraAimAssistActorPriorityPacket.h"

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

CameraAimAssistActorPriorityPacket CameraAimAssistActorPriorityPacket::create(std::vector<types::camera::CameraAimAssistActorPriorityData> priorityData)
{
    CameraAimAssistActorPriorityPacket result;
    result.priorityData = std::move(priorityData);
    return result;
}

void CameraAimAssistActorPriorityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    priorityData.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        priorityData.push_back(types::camera::CameraAimAssistActorPriorityData::read(in));
    }

}

void CameraAimAssistActorPriorityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(priorityData.size()));
    for (const auto &data : priorityData) {
        data.write(out);
    }

}

bool CameraAimAssistActorPriorityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraAimAssistActorPriority(*this);
}

}  // namespace bedrock_protocol
