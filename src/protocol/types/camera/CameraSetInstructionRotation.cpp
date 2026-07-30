/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionRotation.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionRotation.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;

CameraSetInstructionRotation CameraSetInstructionRotation::read(encoding::ByteBufferReader &in)
{
    const auto pitch = LE::readFloat(in);
    const auto yaw = LE::readFloat(in);
    return CameraSetInstructionRotation(pitch, yaw);
}

void CameraSetInstructionRotation::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, pitch);
    LE::writeFloat(out, yaw);
}

}  // namespace bedrock_protocol::types::camera
