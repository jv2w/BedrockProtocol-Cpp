/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraFadeInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraFadeInstruction.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using serializer::CommonTypes;

CameraFadeInstruction CameraFadeInstruction::read(encoding::ByteBufferReader &in)
{
    auto time = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return CameraFadeInstructionTime::read(in); });
    auto color = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return CameraFadeInstructionColor::read(in); });
    return CameraFadeInstruction(std::move(time), std::move(color));
}

void CameraFadeInstruction::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(
        out, time, [](encoding::ByteBufferWriter &out, const CameraFadeInstructionTime &v) { v.write(out); });
    CommonTypes::writeOptional(
        out, color, [](encoding::ByteBufferWriter &out, const CameraFadeInstructionColor &v) { v.write(out); });
}

}  // namespace bedrock_protocol::types::camera
