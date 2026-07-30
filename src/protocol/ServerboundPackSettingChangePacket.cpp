/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerboundPackSettingChangePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerboundPackSettingChangePacket.h"

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

ServerboundPackSettingChangePacket ServerboundPackSettingChangePacket::create(uuid::Uuid packId, std::unique_ptr<types::PackSetting> packSetting)
{
    ServerboundPackSettingChangePacket result;
    result.packId = std::move(packId);
    result.packSetting = std::move(packSetting);
    return result;
}

void ServerboundPackSettingChangePacket::decodePayload(encoding::ByteBufferReader &in)
{
    packId = serializer::CommonTypes::getUUID(in);

    const auto name = serializer::CommonTypes::getString(in);
    const auto typeId = types::PackSettingTypeFromPacket(static_cast<std::int32_t>(encoding::VarInt::readUnsignedInt(in)));
    switch (typeId) {
    case types::PackSettingType::FLOAT:
        packSetting = std::make_unique<types::FloatPackSetting>(types::FloatPackSetting::read(in, name));
        break;
    case types::PackSettingType::BOOL:
        packSetting = std::make_unique<types::BoolPackSetting>(types::BoolPackSetting::read(in, name));
        break;
    case types::PackSettingType::STRING:
        packSetting = std::make_unique<types::StringPackSetting>(types::StringPackSetting::read(in, name));
        break;
    default:
        throw PacketDecodeException("Unhandled pack setting type " + std::to_string(static_cast<std::int32_t>(typeId)));
    }

}

void ServerboundPackSettingChangePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putUUID(out, packId);
    serializer::CommonTypes::putString(out, packSetting->getName());
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(packSetting->getTypeId()));
    packSetting->write(out);

}

bool ServerboundPackSettingChangePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerboundPackSettingChange(*this);
}

}  // namespace bedrock_protocol
