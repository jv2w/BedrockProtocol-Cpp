/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionEase.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionEase.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::camera {

using encoding::Byte;
using encoding::LE;

CameraSetInstructionEase CameraSetInstructionEase::read(encoding::ByteBufferReader &in)
{
    const auto type = Byte::readUnsigned(in);
    const auto duration = LE::readFloat(in);
    return CameraSetInstructionEase(type, duration);
}

void CameraSetInstructionEase::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, type);
    LE::writeFloat(out, duration);
}

}  // namespace bedrock_protocol::types::camera
