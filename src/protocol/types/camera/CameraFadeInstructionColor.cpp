/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstructionColor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraFadeInstructionColor.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;

CameraFadeInstructionColor CameraFadeInstructionColor::read(encoding::ByteBufferReader &in)
{
    const auto red = LE::readFloat(in);
    const auto green = LE::readFloat(in);
    const auto blue = LE::readFloat(in);
    return CameraFadeInstructionColor(red, green, blue);
}

void CameraFadeInstructionColor::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, red);
    LE::writeFloat(out, green);
    LE::writeFloat(out, blue);
}

}  // namespace bedrock_protocol::types::camera
