/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSplineDefinition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSplineDefinition.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using serializer::CommonTypes;

CameraSplineDefinition CameraSplineDefinition::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    auto instruction = CameraSplineInstruction::read(in);
    return CameraSplineDefinition(std::move(name), std::move(instruction));
}

void CameraSplineDefinition::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    instruction.write(out);
}

}  // namespace bedrock_protocol::types::camera
