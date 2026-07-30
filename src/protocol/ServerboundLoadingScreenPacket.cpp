/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ServerboundLoadingScreenPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerboundLoadingScreenPacket.h"

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

ServerboundLoadingScreenPacket ServerboundLoadingScreenPacket::create(types::hud::LoadingScreenType loadingScreenType, std::optional<std::uint32_t> loadingScreenId)
{
    ServerboundLoadingScreenPacket result;
    result.loadingScreenType = std::move(loadingScreenType);
    result.loadingScreenId = std::move(loadingScreenId);
    return result;
}

void ServerboundLoadingScreenPacket::decodePayload(encoding::ByteBufferReader &in)
{
    loadingScreenType = types::hud::LoadingScreenTypeFromPacket(encoding::VarInt::readSignedInt(in));
    loadingScreenId = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readUnsignedInt(reader); });

}

void ServerboundLoadingScreenPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(loadingScreenType));
    serializer::CommonTypes::writeOptional(out, loadingScreenId, [](encoding::ByteBufferWriter &writer, const std::uint32_t &value) { encoding::LE::writeUnsignedInt(writer, value); });

}

bool ServerboundLoadingScreenPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerboundLoadingScreen(*this);
}

}  // namespace bedrock_protocol
