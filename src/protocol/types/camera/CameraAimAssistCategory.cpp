/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistCategory.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategory.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using serializer::CommonTypes;

CameraAimAssistCategory CameraAimAssistCategory::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    auto priorities = CameraAimAssistCategoryPriorities::read(in);
    return CameraAimAssistCategory(std::move(name), std::move(priorities));
}

void CameraAimAssistCategory::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    priorities.write(out);
}

}  // namespace bedrock_protocol::types::camera
