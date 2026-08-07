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

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using encoding::LE;
using serializer::CommonTypes;

MolangItemDescriptor MolangItemDescriptor::read(encoding::ByteBufferReader &in)
{
    auto expression = CommonTypes::getString(in);
    //gophertunnel minecraft/protocol/item_descriptor.go:58-61 - the version widened to a fixed int16.
    const auto version = LE::readSignedShort(in);

    return MolangItemDescriptor(std::move(expression), version);
}

void MolangItemDescriptor::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, molangExpression);
    LE::writeSignedShort(out, molangVersion);
}

}  // namespace bedrock_protocol::types::recipe
