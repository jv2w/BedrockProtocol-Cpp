/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateTradePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateTradePacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

UpdateTradePacket UpdateTradePacket::create(std::uint8_t windowId, std::uint8_t windowType, std::int32_t windowSlotCount, std::int32_t tradeTier, std::int64_t traderActorUniqueId, std::int64_t playerActorUniqueId, std::string displayName, bool isV2Trading, bool isEconomyTrading, types::CacheableNbt<nbt::tag::CompoundTag> offers)
{
    UpdateTradePacket result;
    result.windowId = windowId;
    result.windowType = windowType;
    result.windowSlotCount = windowSlotCount;
    result.tradeTier = tradeTier;
    result.traderActorUniqueId = traderActorUniqueId;
    result.playerActorUniqueId = playerActorUniqueId;
    result.displayName = std::move(displayName);
    result.isV2Trading = isV2Trading;
    result.isEconomyTrading = isEconomyTrading;
    result.offers = std::move(offers);
    return result;
}

void UpdateTradePacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::Byte::readUnsigned(in);
    windowType = encoding::Byte::readUnsigned(in);
    windowSlotCount = encoding::VarInt::readSignedInt(in);
    tradeTier = encoding::VarInt::readSignedInt(in);
    traderActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    playerActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    displayName = serializer::CommonTypes::getString(in);
    isV2Trading = serializer::CommonTypes::getBool(in);
    isEconomyTrading = serializer::CommonTypes::getBool(in);
    offers = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void UpdateTradePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, windowId);
    encoding::Byte::writeUnsigned(out, windowType);
    encoding::VarInt::writeSignedInt(out, windowSlotCount);
    encoding::VarInt::writeSignedInt(out, tradeTier);
    serializer::CommonTypes::putActorUniqueId(out, traderActorUniqueId);
    serializer::CommonTypes::putActorUniqueId(out, playerActorUniqueId);
    serializer::CommonTypes::putString(out, displayName);
    serializer::CommonTypes::putBool(out, isV2Trading);
    serializer::CommonTypes::putBool(out, isEconomyTrading);
    out.writeByteArray(offers.getEncodedNbt());

}

bool UpdateTradePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateTrade(*this);
}

}  // namespace bedrock_protocol
