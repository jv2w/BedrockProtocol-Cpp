/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PurchaseReceiptPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PurchaseReceiptPacket.h"

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

PurchaseReceiptPacket PurchaseReceiptPacket::create(std::vector<std::string> entries)
{
    PurchaseReceiptPacket result;
    result.entries = std::move(entries);
    return result;
}

void PurchaseReceiptPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; ++i) {
        entries.push_back(serializer::CommonTypes::getString(in));
    }

}

void PurchaseReceiptPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        serializer::CommonTypes::putString(out, entry);
    }

}

bool PurchaseReceiptPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePurchaseReceipt(*this);
}

}  // namespace bedrock_protocol
