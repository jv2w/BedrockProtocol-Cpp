/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientCameraAimAssistPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientCameraAimAssistPacket.h"

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

ClientCameraAimAssistPacket ClientCameraAimAssistPacket::create(std::string presetId, types::camera::CameraAimAssistActionType actionType, bool allowAimAssist)
{
    ClientCameraAimAssistPacket result;
    result.presetId = std::move(presetId);
    result.actionType = actionType;
    result.allowAimAssist = allowAimAssist;
    return result;
}

void ClientCameraAimAssistPacket::decodePayload(encoding::ByteBufferReader &in)
{
    presetId = serializer::CommonTypes::getString(in);
    actionType = types::camera::CameraAimAssistActionTypeFromPacket(encoding::Byte::readUnsigned(in));
    allowAimAssist = serializer::CommonTypes::getBool(in);

}

void ClientCameraAimAssistPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, presetId);
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(actionType));
    serializer::CommonTypes::putBool(out, allowAimAssist);

}

bool ClientCameraAimAssistPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientCameraAimAssist(*this);
}

}  // namespace bedrock_protocol
