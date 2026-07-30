/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstructionTime.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraFadeInstructionTime.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;

CameraFadeInstructionTime CameraFadeInstructionTime::read(encoding::ByteBufferReader &in)
{
    const auto fadeInTime = LE::readFloat(in);
    const auto stayTime = LE::readFloat(in);
    const auto fadeOutTime = LE::readFloat(in);
    return CameraFadeInstructionTime(fadeInTime, stayTime, fadeOutTime);
}

void CameraFadeInstructionTime::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, fadeInTime);
    LE::writeFloat(out, stayTime);
    LE::writeFloat(out, fadeOutTime);
}

}  // namespace bedrock_protocol::types::camera
