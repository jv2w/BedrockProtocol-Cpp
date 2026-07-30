/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSplineInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSplineInstruction.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

CameraSplineInstruction CameraSplineInstruction::read(encoding::ByteBufferReader &in)
{
    const auto totalTime = LE::readFloat(in);
    const auto easeType = Byte::readUnsigned(in);

    std::vector<math::Vector3> curve;
    const auto curveCount = VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < curveCount; ++i) {
        curve.push_back(CommonTypes::getVector3(in));
    }

    std::vector<CameraProgressOption> progressKeyFrames;
    const auto progressKeyFrameCount = VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < progressKeyFrameCount; ++i) {
        progressKeyFrames.push_back(CameraProgressOption::read(in));
    }

    std::vector<CameraRotationOption> rotationOptions;
    const auto rotationOptionCount = VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < rotationOptionCount; ++i) {
        rotationOptions.push_back(CameraRotationOption::read(in));
    }

    auto splineIdentifier = CommonTypes::getString(in);
    const auto loadFromJson = CommonTypes::getBool(in);

    return CameraSplineInstruction(totalTime, easeType, std::move(curve), std::move(progressKeyFrames),
                                   std::move(rotationOptions), std::move(splineIdentifier), loadFromJson);
}

void CameraSplineInstruction::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, totalTime);
    Byte::writeUnsigned(out, easeType);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(curve.size()));
    for (const auto &point : curve) {
        CommonTypes::putVector3(out, point);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(progressKeyFrames.size()));
    for (const auto &keyFrame : progressKeyFrames) {
        keyFrame.write(out);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(rotationOptions.size()));
    for (const auto &option : rotationOptions) {
        option.write(out);
    }

    CommonTypes::putString(out, splineIdentifier);
    CommonTypes::putBool(out, loadFromJson);
}

}  // namespace bedrock_protocol::types::camera
