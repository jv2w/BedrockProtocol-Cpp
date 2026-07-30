/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/MetadataProperty.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/types/entity/BlockPosMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/ByteMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/CompoundTagMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/FloatMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/IntMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/LongMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/ShortMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/StringMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/Vec3MetadataProperty.h"

namespace bedrock_protocol::types::entity {

std::unique_ptr<MetadataProperty> MetadataProperty::read(encoding::ByteBufferReader &in, std::int32_t type)
{
    switch (type) {
    case ByteMetadataProperty::ID:
        return std::make_unique<ByteMetadataProperty>(ByteMetadataProperty::read(in));
    case ShortMetadataProperty::ID:
        return std::make_unique<ShortMetadataProperty>(ShortMetadataProperty::read(in));
    case IntMetadataProperty::ID:
        return std::make_unique<IntMetadataProperty>(IntMetadataProperty::read(in));
    case FloatMetadataProperty::ID:
        return std::make_unique<FloatMetadataProperty>(FloatMetadataProperty::read(in));
    case StringMetadataProperty::ID:
        return std::make_unique<StringMetadataProperty>(StringMetadataProperty::read(in));
    case CompoundTagMetadataProperty::ID:
        return std::make_unique<CompoundTagMetadataProperty>(CompoundTagMetadataProperty::read(in));
    case BlockPosMetadataProperty::ID:
        return std::make_unique<BlockPosMetadataProperty>(BlockPosMetadataProperty::read(in));
    case LongMetadataProperty::ID:
        return std::make_unique<LongMetadataProperty>(LongMetadataProperty::read(in));
    case Vec3MetadataProperty::ID:
        return std::make_unique<Vec3MetadataProperty>(Vec3MetadataProperty::read(in));
    default:
        throw PacketDecodeException("Unknown entity metadata type " + std::to_string(type));
    }
}

}  // namespace bedrock_protocol::types::entity
