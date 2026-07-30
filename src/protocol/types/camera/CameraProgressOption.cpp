/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraProgressOption.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraProgressOption.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::camera {

using encoding::LE;
using serializer::CommonTypes;

CameraProgressOption CameraProgressOption::read(encoding::ByteBufferReader &in)
{
    const auto value = LE::readFloat(in);
    const auto time = LE::readFloat(in);
    auto easeType = CommonTypes::getString(in);

    return CameraProgressOption(value, time, std::move(easeType));
}

void CameraProgressOption::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, value);
    LE::writeFloat(out, time);
    CommonTypes::putString(out, easeType);
}

}  // namespace bedrock_protocol::types::camera
