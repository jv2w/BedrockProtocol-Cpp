/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CameraInstructionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraInstructionPacket.h"

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

CameraInstructionPacket CameraInstructionPacket::create(std::optional<types::camera::CameraSetInstruction> set, std::optional<bool> clear, std::optional<types::camera::CameraFadeInstruction> fade, std::optional<types::camera::CameraTargetInstruction> target, std::optional<bool> removeTarget, std::optional<types::camera::CameraFovInstruction> fieldOfView, std::optional<types::camera::CameraSplineInstruction> spline, std::optional<std::int64_t> attachToEntity, std::optional<bool> detachFromEntity)
{
    CameraInstructionPacket result;
    result.set = std::move(set);
    result.clear = std::move(clear);
    result.fade = std::move(fade);
    result.target = std::move(target);
    result.removeTarget = std::move(removeTarget);
    result.fieldOfView = std::move(fieldOfView);
    result.spline = std::move(spline);
    result.attachToEntity = std::move(attachToEntity);
    result.detachFromEntity = std::move(detachFromEntity);
    return result;
}

void CameraInstructionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    set = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::camera::CameraSetInstruction::read(reader); });
    clear = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getBool(reader); });
    fade = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::camera::CameraFadeInstruction::read(reader); });
    target = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::camera::CameraTargetInstruction::read(reader); });
    removeTarget = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getBool(reader); });
    fieldOfView = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::camera::CameraFovInstruction::read(reader); });
    spline = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::camera::CameraSplineInstruction::read(reader); });
    attachToEntity = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readSignedLong(reader); }); //WHY IS THIS NON-STANDARD?
    detachFromEntity = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getBool(reader); });

}

void CameraInstructionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(out, set, [](encoding::ByteBufferWriter &writer, const types::camera::CameraSetInstruction &v) { v.write(writer); });
    serializer::CommonTypes::writeOptional(out, clear, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putBool(writer, value); });
    serializer::CommonTypes::writeOptional(out, fade, [](encoding::ByteBufferWriter &writer, const types::camera::CameraFadeInstruction &v) { v.write(writer); });
    serializer::CommonTypes::writeOptional(out, target, [](encoding::ByteBufferWriter &writer, const types::camera::CameraTargetInstruction &v) { v.write(writer); });
    serializer::CommonTypes::writeOptional(out, removeTarget, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putBool(writer, value); });
    serializer::CommonTypes::writeOptional(out, fieldOfView, [](encoding::ByteBufferWriter &writer, const types::camera::CameraFovInstruction &v) { v.write(writer); });
    serializer::CommonTypes::writeOptional(out, spline, [](encoding::ByteBufferWriter &writer, const types::camera::CameraSplineInstruction &v) { v.write(writer); });
    serializer::CommonTypes::writeOptional(out, attachToEntity, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeSignedLong(writer, value); }); //WHY IS THIS NON-STANDARD?
    serializer::CommonTypes::writeOptional(out, detachFromEntity, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putBool(writer, value); });

}

bool CameraInstructionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraInstruction(*this);
}

}  // namespace bedrock_protocol
