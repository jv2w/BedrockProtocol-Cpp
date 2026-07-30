/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EducationUriResource.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/EducationUriResource.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

EducationUriResource EducationUriResource::read(encoding::ByteBufferReader &in)
{
    auto buttonName = CommonTypes::getString(in);
    auto linkUri = CommonTypes::getString(in);
    return EducationUriResource(std::move(buttonName), std::move(linkUri));
}

void EducationUriResource::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, buttonName);
    CommonTypes::putString(out, linkUri);
}

}  // namespace bedrock_protocol::types
