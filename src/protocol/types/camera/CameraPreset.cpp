/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraPreset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraPreset.h"

#include <cstdint>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

CameraPreset CameraPreset::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    auto parent = CommonTypes::getString(in);
    const auto xPosition = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto yPosition = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto zPosition = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto pitch = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto yaw = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto rotationSpeed =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto snapToTarget =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    auto horizontalRotationLimit =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    auto verticalRotationLimit =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    const auto continueTargeting =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    const auto blockListeningRadius =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    auto viewOffset =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    auto entityOffset =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector3(in); });
    const auto radius = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto yawLimitMin =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto yawLimitMax =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });
    const auto audioListenerType =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return Byte::readUnsigned(in); });
    const auto playerEffects =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    auto aimAssist =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CameraPresetAimAssist::read(in); });
    const auto controlScheme = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return ControlSchemeFromPacket(Byte::readUnsigned(in)); });

    return CameraPreset(
        std::move(name),
        std::move(parent),
        xPosition,
        yPosition,
        zPosition,
        pitch,
        yaw,
        rotationSpeed,
        snapToTarget,
        std::move(horizontalRotationLimit),
        std::move(verticalRotationLimit),
        continueTargeting,
        blockListeningRadius,
        std::move(viewOffset),
        std::move(entityOffset),
        radius,
        yawLimitMin,
        yawLimitMax,
        audioListenerType,
        playerEffects,
        std::move(aimAssist),
        controlScheme
    );
}

void CameraPreset::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    CommonTypes::putString(out, parent);
    CommonTypes::writeOptional(out, xPosition, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, yPosition, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, zPosition, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, pitch, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, yaw, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, rotationSpeed,
                               [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, snapToTarget,
                               [](encoding::ByteBufferWriter &out, bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::writeOptional(out, horizontalRotationLimit,
                               [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
                                   CommonTypes::putVector2(out, v);
                               });
    CommonTypes::writeOptional(out, verticalRotationLimit, [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
        CommonTypes::putVector2(out, v);
    });
    CommonTypes::writeOptional(out, continueTargeting,
                               [](encoding::ByteBufferWriter &out, bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::writeOptional(out, blockListeningRadius,
                               [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, viewOffset, [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
        CommonTypes::putVector2(out, v);
    });
    CommonTypes::writeOptional(out, entityOffset, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, radius, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, yawLimitMin,
                               [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, yawLimitMax,
                               [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
    CommonTypes::writeOptional(out, audioListenerType,
                               [](encoding::ByteBufferWriter &out, std::uint8_t v) { Byte::writeUnsigned(out, v); });
    CommonTypes::writeOptional(out, playerEffects,
                               [](encoding::ByteBufferWriter &out, bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::writeOptional(
        out, aimAssist, [](encoding::ByteBufferWriter &out, const CameraPresetAimAssist &v) { v.write(out); });
    CommonTypes::writeOptional(out, controlScheme, [](encoding::ByteBufferWriter &out, ControlScheme v) {
        Byte::writeUnsigned(out, static_cast<std::uint8_t>(v));
    });
}

}  // namespace bedrock_protocol::types::camera
