/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePackClientResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackClientResponsePacket.h"

#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {

/**
 * gophertunnel v1.58.0 minecraft/protocol/packet/resource_pack_client_response.go:45 writes this name
 * after the numeric response, indexed by the response itself.
 */
constexpr std::string_view RESPONSE_NAMES[] = {"cancel", "downloading", "downloadingfinished",
                                               "resourcepackstackfinished"};

}  // namespace

ResourcePackClientResponsePacket ResourcePackClientResponsePacket::create(std::uint32_t status, std::vector<std::string> packIds)
{
    ResourcePackClientResponsePacket result;
    result.status = status;
    result.packIds = std::move(packIds);
    return result;
}

void ResourcePackClientResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    status = encoding::VarInt::readUnsignedInt(in);
    if (status >= std::size(RESPONSE_NAMES)) {
        throw PacketDecodeException("Unknown resource pack response " + std::to_string(status));
    }
    // The name is redundant with the numeric response, so it is read and discarded.
    serializer::CommonTypes::getString(in);

    packIds.clear();
    if (status == STATUS_SEND_PACKS) {
        for (auto entryCount = encoding::VarInt::readUnsignedInt(in); entryCount-- > 0;) {
            packIds.push_back(serializer::CommonTypes::getString(in));
        }
    }
}

void ResourcePackClientResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    if (status >= std::size(RESPONSE_NAMES)) {
        throw std::invalid_argument("Unknown resource pack response " + std::to_string(status));
    }
    encoding::VarInt::writeUnsignedInt(out, status);
    serializer::CommonTypes::putString(out, std::string(RESPONSE_NAMES[status]));

    if (status == STATUS_SEND_PACKS) {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(packIds.size()));
        for (const auto &id : packIds) {
            serializer::CommonTypes::putString(out, id);
        }
    }
}

bool ResourcePackClientResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackClientResponse(*this);
}

}  // namespace bedrock_protocol
