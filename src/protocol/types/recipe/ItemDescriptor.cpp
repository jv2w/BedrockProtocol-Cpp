/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/ItemDescriptor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/recipe/ItemDescriptor.h"

#include "bedrock_protocol/protocol/types/recipe/ComplexAliasItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/IntIdMetaItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptorType.h"
#include "bedrock_protocol/protocol/types/recipe/MolangItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/StringIdMetaItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/TagItemDescriptor.h"

namespace bedrock_protocol::types::recipe {

std::unique_ptr<ItemDescriptor> ItemDescriptor::read(encoding::ByteBufferReader &in, std::uint8_t descriptorType)
{
    switch (descriptorType) {
    case ItemDescriptorType::INT_ID_META:
        return std::make_unique<IntIdMetaItemDescriptor>(IntIdMetaItemDescriptor::read(in));
    case ItemDescriptorType::STRING_ID_META:
        return std::make_unique<StringIdMetaItemDescriptor>(StringIdMetaItemDescriptor::read(in));
    case ItemDescriptorType::TAG:
        return std::make_unique<TagItemDescriptor>(TagItemDescriptor::read(in));
    case ItemDescriptorType::MOLANG:
        return std::make_unique<MolangItemDescriptor>(MolangItemDescriptor::read(in));
    case ItemDescriptorType::COMPLEX_ALIAS:
        return std::make_unique<ComplexAliasItemDescriptor>(ComplexAliasItemDescriptor::read(in));
    default:
        return nullptr;
    }
}

}  // namespace bedrock_protocol::types::recipe
