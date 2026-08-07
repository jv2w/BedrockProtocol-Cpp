/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponse.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponse.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory::stackresponse {

using encoding::Byte;
using encoding::VarInt;
using serializer::CommonTypes;

ItemStackResponse::ItemStackResponse(const std::uint8_t result, const std::int32_t requestId,
                                     std::vector<ItemStackResponseContainerInfo> containerInfos)
    : result(result), requestId(requestId), containerInfos(std::move(containerInfos))
{
}

ItemStackResponse ItemStackResponse::read(encoding::ByteBufferReader &in)
{
    const auto result = Byte::readUnsigned(in);
    const auto requestId = CommonTypes::readItemStackRequestId(in);
    //gophertunnel minecraft/protocol/item_stack.go:231-244 - the container list is a double optional whose
    //presence is driven by emptiness rather than by the status.
    std::vector<ItemStackResponseContainerInfo> containerInfos;
    if (CommonTypes::getBool(in) && CommonTypes::getBool(in)) {
        for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
            containerInfos.push_back(ItemStackResponseContainerInfo::read(in));
        }
    }
    return ItemStackResponse(result, requestId, std::move(containerInfos));
}

void ItemStackResponse::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, result);
    CommonTypes::writeItemStackRequestId(out, requestId);
    //The outer bool of a double optional is always written as true.
    CommonTypes::putBool(out, true);
    const auto present = !containerInfos.empty();
    CommonTypes::putBool(out, present);
    if (present) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(containerInfos.size()));
        for (const auto &containerInfo : containerInfos) {
            containerInfo.write(out);
        }
    }
}

}  // namespace bedrock_protocol::types::inventory::stackresponse
