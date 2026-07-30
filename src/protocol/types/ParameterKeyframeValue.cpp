/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ParameterKeyframeValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ParameterKeyframeValue.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

ParameterKeyframeValue ParameterKeyframeValue::read(encoding::ByteBufferReader &in)
{
    const auto time = LE::readFloat(in);
    const auto value = CommonTypes::getVector3(in);

    return ParameterKeyframeValue(time, value);
}

void ParameterKeyframeValue::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, time);
    CommonTypes::putVector3(out, value);
}

}  // namespace bedrock_protocol::types
