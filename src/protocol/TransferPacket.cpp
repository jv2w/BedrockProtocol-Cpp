/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/TransferPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/TransferPacket.h"

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

TransferPacket TransferPacket::create(std::string address, std::uint16_t port, bool reloadWorld, std::optional<types::GatheringJoinInfo> gatheringJoinInfo)
{
    TransferPacket result;
    result.address = std::move(address);
    result.port = port;
    result.reloadWorld = reloadWorld;
    result.gatheringJoinInfo = std::move(gatheringJoinInfo);
    return result;
}

void TransferPacket::decodePayload(encoding::ByteBufferReader &in)
{
    address = serializer::CommonTypes::getString(in);
    port = encoding::LE::readUnsignedShort(in);
    reloadWorld = serializer::CommonTypes::getBool(in);
    gatheringJoinInfo = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::GatheringJoinInfo::read(reader); });

}

void TransferPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, address);
    encoding::LE::writeUnsignedShort(out, port);
    serializer::CommonTypes::putBool(out, reloadWorld);
    serializer::CommonTypes::writeOptional(out, gatheringJoinInfo, [](encoding::ByteBufferWriter &writer, const types::GatheringJoinInfo &info) { info.write(writer); });

}

bool TransferPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleTransfer(*this);
}

}  // namespace bedrock_protocol
