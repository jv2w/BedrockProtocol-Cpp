/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraPresetAimAssist.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraPresetAimAssist.h"

#include <cstdint>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

CameraPresetAimAssist CameraPresetAimAssist::read(encoding::ByteBufferReader &in)
{
    auto presetId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getString(in); });
    const auto targetMode = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CameraAimAssistTargetModeFromPacket(Byte::readUnsigned(in));
    });
    auto viewAngle =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    const auto distance = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readFloat(in); });

    return CameraPresetAimAssist(std::move(presetId), targetMode, std::move(viewAngle), distance);
}

void CameraPresetAimAssist::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(out, presetId, [](encoding::ByteBufferWriter &out, const std::string &v) {
        CommonTypes::putString(out, v);
    });
    CommonTypes::writeOptional(out, targetMode, [](encoding::ByteBufferWriter &out, CameraAimAssistTargetMode v) {
        Byte::writeUnsigned(out, static_cast<std::uint8_t>(v));
    });
    CommonTypes::writeOptional(out, viewAngle, [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
        CommonTypes::putVector2(out, v);
    });
    CommonTypes::writeOptional(out, distance, [](encoding::ByteBufferWriter &out, float v) { LE::writeFloat(out, v); });
}

}  // namespace bedrock_protocol::types::camera
