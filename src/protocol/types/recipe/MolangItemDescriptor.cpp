/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/MolangItemDescriptor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/MolangItemDescriptor.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::Byte;
using serializer::CommonTypes;

MolangItemDescriptor MolangItemDescriptor::read(encoding::ByteBufferReader &in)
{
    auto expression = CommonTypes::getString(in);
    const auto version = Byte::readUnsigned(in);

    return MolangItemDescriptor(std::move(expression), version);
}

void MolangItemDescriptor::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, molangExpression);
    Byte::writeUnsigned(out, molangVersion);
}

}  // namespace bedrock_protocol::types::recipe
