/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistActorPriorityData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistActorPriorityData.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;

CameraAimAssistActorPriorityData CameraAimAssistActorPriorityData::read(encoding::ByteBufferReader &in)
{
    const auto presetIndex = LE::readSignedInt(in);
    const auto categoryIndex = LE::readSignedInt(in);
    const auto actorIndex = LE::readSignedInt(in);
    const auto priority = LE::readSignedInt(in);
    return CameraAimAssistActorPriorityData(presetIndex, categoryIndex, actorIndex, priority);
}

void CameraAimAssistActorPriorityData::write(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedInt(out, presetIndex);
    LE::writeSignedInt(out, categoryIndex);
    LE::writeSignedInt(out, actorIndex);
    LE::writeSignedInt(out, priority);
}

}  // namespace bedrock_protocol::types::camera
