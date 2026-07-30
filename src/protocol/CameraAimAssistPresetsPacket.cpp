/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CameraAimAssistPresetsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraAimAssistPresetsPacket.h"

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

CameraAimAssistPresetsPacket CameraAimAssistPresetsPacket::create(std::vector<types::camera::CameraAimAssistCategory> categories, std::vector<types::camera::CameraAimAssistPreset> presets, std::uint8_t operation)
{
    CameraAimAssistPresetsPacket result;
    result.categories = std::move(categories);
    result.presets = std::move(presets);
    result.operation = operation;
    return result;
}

void CameraAimAssistPresetsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    categories.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        categories.push_back(types::camera::CameraAimAssistCategory::read(in));
    }

    presets.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        presets.push_back(types::camera::CameraAimAssistPreset::read(in));
    }

    operation = encoding::Byte::readUnsigned(in);

}

void CameraAimAssistPresetsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(categories.size()));
    for (const auto &category : categories) {
        category.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(presets.size()));
    for (const auto &preset : presets) {
        preset.write(out);
    }

    encoding::Byte::writeUnsigned(out, operation);

}

bool CameraAimAssistPresetsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraAimAssistPresets(*this);
}

}  // namespace bedrock_protocol
