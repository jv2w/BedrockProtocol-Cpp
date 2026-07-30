/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CameraPresetsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraPresetsPacket.h"

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

CameraPresetsPacket CameraPresetsPacket::create(std::vector<types::camera::CameraPreset> presets)
{
    CameraPresetsPacket result;
    result.presets = std::move(presets);
    return result;
}

void CameraPresetsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    presets.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        presets.push_back(types::camera::CameraPreset::read(in));
    }

}

void CameraPresetsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(presets.size()));
    for (const auto &preset : presets) {
        preset.write(out);
    }

}

bool CameraPresetsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraPresets(*this);
}

}  // namespace bedrock_protocol
