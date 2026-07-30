/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/NetworkPermissions.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/NetworkPermissions.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

NetworkPermissions NetworkPermissions::decode(encoding::ByteBufferReader &in)
{
    const auto disableClientSounds = CommonTypes::getBool(in);
    return NetworkPermissions(disableClientSounds);
}

void NetworkPermissions::encode(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBool(out, disableClientSounds_);
}

}  // namespace bedrock_protocol::types
