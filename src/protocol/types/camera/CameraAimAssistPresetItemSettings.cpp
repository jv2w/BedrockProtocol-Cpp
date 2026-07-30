/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistPresetItemSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPresetItemSettings.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using serializer::CommonTypes;

CameraAimAssistPresetItemSettings CameraAimAssistPresetItemSettings::read(encoding::ByteBufferReader &in)
{
    auto itemIdentifier = CommonTypes::getString(in);
    auto categoryName = CommonTypes::getString(in);
    return CameraAimAssistPresetItemSettings(std::move(itemIdentifier), std::move(categoryName));
}

void CameraAimAssistPresetItemSettings::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, itemIdentifier);
    CommonTypes::putString(out, categoryName);
}

}  // namespace bedrock_protocol::types::camera
