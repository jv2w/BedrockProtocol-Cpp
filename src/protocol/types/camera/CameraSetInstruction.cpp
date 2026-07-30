/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSetInstruction.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using serializer::CommonTypes;

CameraSetInstruction CameraSetInstruction::read(encoding::ByteBufferReader &in)
{
    const auto preset = LE::readUnsignedInt(in);
    auto ease =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CameraSetInstructionEase::read(in); });
    auto cameraPosition =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector3(in); });
    auto rotation = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return CameraSetInstructionRotation::read(in); });
    auto facingPosition =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector3(in); });
    auto viewOffset =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    auto entityOffset =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector3(in); });
    auto default_ =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    const auto ignoreStartingValuesComponent = CommonTypes::getBool(in);

    return CameraSetInstruction(preset, std::move(ease), std::move(cameraPosition), std::move(rotation),
                                std::move(facingPosition), std::move(viewOffset), std::move(entityOffset), default_,
                                ignoreStartingValuesComponent);
}

void CameraSetInstruction::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, preset);
    CommonTypes::writeOptional(
        out, ease, [](encoding::ByteBufferWriter &out, const CameraSetInstructionEase &v) { v.write(out); });
    CommonTypes::writeOptional(out, cameraPosition, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(
        out, rotation, [](encoding::ByteBufferWriter &out, const CameraSetInstructionRotation &v) { v.write(out); });
    CommonTypes::writeOptional(out, facingPosition, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, viewOffset, [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
        CommonTypes::putVector2(out, v);
    });
    CommonTypes::writeOptional(out, entityOffset, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, default_,
                               [](encoding::ByteBufferWriter &out, bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::putBool(out, ignoreStartingValuesComponent);
}

}  // namespace bedrock_protocol::types::camera
