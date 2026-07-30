/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraTargetInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraTargetInstruction.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using serializer::CommonTypes;

CameraTargetInstruction CameraTargetInstruction::read(encoding::ByteBufferReader &in)
{
    auto targetCenterOffset =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector3(in); });
    const auto actorUniqueId = LE::readSignedLong(in);  //why be consistent mojang ?????
    return CameraTargetInstruction(std::move(targetCenterOffset), actorUniqueId);
}

void CameraTargetInstruction::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(out, targetCenterOffset, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    LE::writeSignedLong(out, actorUniqueId);
}

}  // namespace bedrock_protocol::types::camera
