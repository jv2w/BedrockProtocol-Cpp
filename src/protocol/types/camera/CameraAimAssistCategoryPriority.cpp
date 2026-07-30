/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistCategoryPriority.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategoryPriority.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using serializer::CommonTypes;

CameraAimAssistCategoryPriority CameraAimAssistCategoryPriority::read(encoding::ByteBufferReader &in)
{
    auto identifier = CommonTypes::getString(in);
    const auto priority = LE::readSignedInt(in);
    return CameraAimAssistCategoryPriority(std::move(identifier), priority);
}

void CameraAimAssistCategoryPriority::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, identifier);
    LE::writeSignedInt(out, priority);
}

}  // namespace bedrock_protocol::types::camera
