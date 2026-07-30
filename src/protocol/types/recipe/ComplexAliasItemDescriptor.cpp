/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/ComplexAliasItemDescriptor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/ComplexAliasItemDescriptor.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::recipe {

using serializer::CommonTypes;

ComplexAliasItemDescriptor ComplexAliasItemDescriptor::read(encoding::ByteBufferReader &in)
{
    auto alias = CommonTypes::getString(in);

    return ComplexAliasItemDescriptor(std::move(alias));
}

void ComplexAliasItemDescriptor::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, alias);
}

}  // namespace bedrock_protocol::types::recipe
