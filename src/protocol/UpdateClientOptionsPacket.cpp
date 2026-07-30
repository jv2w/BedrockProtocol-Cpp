/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateClientOptionsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateClientOptionsPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/GraphicsMode.h"

namespace bedrock_protocol {

UpdateClientOptionsPacket UpdateClientOptionsPacket::create(std::optional<types::GraphicsMode> graphicsMode, std::optional<bool> filterProfanityChange)
{
    UpdateClientOptionsPacket result;
    result.graphicsMode = std::move(graphicsMode);
    result.filterProfanityChange = std::move(filterProfanityChange);
    return result;
}

void UpdateClientOptionsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    graphicsMode = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return types::GraphicsModeFromPacket(encoding::Byte::readUnsigned(in)); });
    filterProfanityChange = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getBool(reader); });

}

void UpdateClientOptionsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::writeOptional(out, graphicsMode, [](encoding::ByteBufferWriter &out, const types::GraphicsMode &v) { encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(v)); });
    serializer::CommonTypes::writeOptional(out, filterProfanityChange, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putBool(writer, value); });

}

bool UpdateClientOptionsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateClientOptions(*this);
}

}  // namespace bedrock_protocol
