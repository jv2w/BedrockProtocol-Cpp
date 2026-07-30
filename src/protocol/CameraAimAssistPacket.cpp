/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CameraAimAssistPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraAimAssistPacket.h"

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

CameraAimAssistPacket CameraAimAssistPacket::create(std::string presetId, math::Vector2 viewAngle, float distance, types::camera::CameraAimAssistTargetMode targetMode, types::camera::CameraAimAssistActionType actionType, bool showDebugRender)
{
    CameraAimAssistPacket result;
    result.presetId = std::move(presetId);
    result.viewAngle = std::move(viewAngle);
    result.distance = distance;
    result.targetMode = targetMode;
    result.actionType = actionType;
    result.showDebugRender = showDebugRender;
    return result;
}

void CameraAimAssistPacket::decodePayload(encoding::ByteBufferReader &in)
{
    presetId = serializer::CommonTypes::getString(in);
    viewAngle = serializer::CommonTypes::getVector2(in);
    distance = encoding::LE::readFloat(in);
    targetMode = types::camera::CameraAimAssistTargetModeFromPacket(encoding::Byte::readUnsigned(in));
    actionType = types::camera::CameraAimAssistActionTypeFromPacket(encoding::Byte::readUnsigned(in));
    showDebugRender = serializer::CommonTypes::getBool(in);

}

void CameraAimAssistPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, presetId);
    serializer::CommonTypes::putVector2(out, viewAngle);
    encoding::LE::writeFloat(out, distance);
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(targetMode));
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(actionType));
    serializer::CommonTypes::putBool(out, showDebugRender);

}

bool CameraAimAssistPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraAimAssist(*this);
}

}  // namespace bedrock_protocol
