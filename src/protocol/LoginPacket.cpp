/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/LoginPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LoginPacket.h"

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

LoginPacket LoginPacket::create(std::uint32_t protocol, std::string authInfoJson, std::string clientDataJwt)
{
    LoginPacket result;
    result.protocol = protocol;
    result.authInfoJson = std::move(authInfoJson);
    result.clientDataJwt = std::move(clientDataJwt);
    return result;
}

void LoginPacket::decodePayload(encoding::ByteBufferReader &in)
{
    protocol = encoding::BE::readUnsignedInt(in);
    decodeConnectionRequest(serializer::CommonTypes::getString(in));

}

void LoginPacket::decodeConnectionRequest(std::string_view binary)
{
    encoding::ByteBufferReader connRequestReader(binary);

    const auto authInfoJsonLength = encoding::LE::readUnsignedInt(connRequestReader);
    authInfoJson = connRequestReader.readByteArray(authInfoJsonLength);

    const auto clientDataJwtLength = encoding::LE::readUnsignedInt(connRequestReader);
    clientDataJwt = connRequestReader.readByteArray(clientDataJwtLength);
}

void LoginPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::BE::writeUnsignedInt(out, protocol);
    serializer::CommonTypes::putString(out, encodeConnectionRequest());

}

std::string LoginPacket::encodeConnectionRequest() const
{
    encoding::ByteBufferWriter connRequestWriter;

    encoding::LE::writeUnsignedInt(connRequestWriter, static_cast<std::uint32_t>(authInfoJson.size()));
    connRequestWriter.writeByteArray(authInfoJson);

    encoding::LE::writeUnsignedInt(connRequestWriter, static_cast<std::uint32_t>(clientDataJwt.size()));
    connRequestWriter.writeByteArray(clientDataJwt);

    return connRequestWriter.getData();
}

bool LoginPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLogin(*this);
}

}  // namespace bedrock_protocol
