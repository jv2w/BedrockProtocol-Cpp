/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraFovInstruction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraFovInstruction.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using serializer::CommonTypes;

CameraFovInstruction CameraFovInstruction::read(encoding::ByteBufferReader &in)
{
    const auto fieldOfView = LE::readFloat(in);
    const auto easeTime = LE::readFloat(in);
    auto easeType = CommonTypes::getString(in);
    const auto clear = CommonTypes::getBool(in);

    return CameraFovInstruction(fieldOfView, easeTime, std::move(easeType), clear);
}

void CameraFovInstruction::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, fieldOfView);
    LE::writeFloat(out, easeTime);
    CommonTypes::putString(out, easeType);
    CommonTypes::putBool(out, clear);
}

}  // namespace bedrock_protocol::types::camera
