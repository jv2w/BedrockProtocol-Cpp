/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponse.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponse.h"

#include <stdexcept>

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
    if (this->result != RESULT_OK && this->containerInfos.size() != 0) {
        throw std::invalid_argument("Container infos must be empty if rejecting the request");
    }
}

ItemStackResponse ItemStackResponse::read(encoding::ByteBufferReader &in)
{
    const auto result = Byte::readUnsigned(in);
    const auto requestId = CommonTypes::readItemStackRequestId(in);
    std::vector<ItemStackResponseContainerInfo> containerInfos;
    if (result == RESULT_OK) {
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
    if (result == RESULT_OK) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(containerInfos.size()));
        for (const auto &containerInfo : containerInfos) {
            containerInfo.write(out);
        }
    }
}

}  // namespace bedrock_protocol::types::inventory::stackresponse
