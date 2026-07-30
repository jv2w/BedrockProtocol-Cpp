/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/FeatureRegistryPacketEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/FeatureRegistryPacketEntry.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

FeatureRegistryPacketEntry FeatureRegistryPacketEntry::read(encoding::ByteBufferReader &in)
{
    auto featureName = CommonTypes::getString(in);
    auto featureJson = CommonTypes::getString(in);

    return FeatureRegistryPacketEntry(std::move(featureName), std::move(featureJson));
}

void FeatureRegistryPacketEntry::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, featureName);
    CommonTypes::putString(out, featureJson);
}

}  // namespace bedrock_protocol::types
