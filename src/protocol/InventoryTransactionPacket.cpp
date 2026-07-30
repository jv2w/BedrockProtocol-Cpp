/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/InventoryTransactionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

InventoryTransactionPacket InventoryTransactionPacket::create(std::int32_t requestId, std::optional<std::vector<types::inventory::InventoryTransactionChangedSlotsHack>> requestChangedSlots, std::unique_ptr<types::inventory::TransactionData> trData)
{
    InventoryTransactionPacket result;
    result.requestId = requestId;
    result.requestChangedSlots = std::move(requestChangedSlots);
    result.trData = std::move(trData);
    return result;
}

void InventoryTransactionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    requestId = serializer::CommonTypes::readLegacyItemStackRequestId(in);

    requestChangedSlots = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        std::vector<types::inventory::InventoryTransactionChangedSlotsHack> result;
        for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
            result.push_back(types::inventory::InventoryTransactionChangedSlotsHack::read(in));
        }
        return result;
    });

    if (encoding::Byte::readUnsigned(in) != 1) {
        throw PacketDecodeException("Dummy optional bool for transactionType should always be 1");
    }
    const auto transactionType = encoding::VarInt::readUnsignedInt(in);
    if (encoding::Byte::readUnsigned(in) != 1) {
        throw PacketDecodeException("Dummy optional bool for trData should always be 1");
    }
    const auto typeId = static_cast<std::int32_t>(transactionType);
    if (typeId == types::inventory::NormalTransactionData::ID) {
        trData = std::make_unique<types::inventory::NormalTransactionData>();
    }
    else if (typeId == types::inventory::MismatchTransactionData::ID) {
        trData = std::make_unique<types::inventory::MismatchTransactionData>();
    }
    else if (typeId == types::inventory::UseItemTransactionData::ID) {
        trData = std::make_unique<types::inventory::UseItemTransactionData>();
    }
    else if (typeId == types::inventory::UseItemOnEntityTransactionData::ID) {
        trData = std::make_unique<types::inventory::UseItemOnEntityTransactionData>();
    }
    else if (typeId == types::inventory::ReleaseItemTransactionData::ID) {
        trData = std::make_unique<types::inventory::ReleaseItemTransactionData>();
    }
    else {
        throw PacketDecodeException("Unknown transaction type " + std::to_string(transactionType));
    }
    trData->decodeTransaction(in);

}

void InventoryTransactionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeLegacyItemStackRequestId(out, requestId);

    serializer::CommonTypes::writeOptional(out, requestChangedSlots, [](encoding::ByteBufferWriter &out, const std::vector<types::inventory::InventoryTransactionChangedSlotsHack> &value) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(value.size()));
        for (const auto &changedSlots : value) {
            changedSlots.write(out);
        }
    });

    encoding::Byte::writeUnsigned(out, 1);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(trData->getTypeId()));
    encoding::Byte::writeUnsigned(out, 1);
    trData->encodeTransaction(out);

}

bool InventoryTransactionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleInventoryTransaction(*this);
}

}  // namespace bedrock_protocol
