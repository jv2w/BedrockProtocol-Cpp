/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ServerboundDiagnosticsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ServerboundDiagnosticsPacket.h"

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

ServerboundDiagnosticsPacket ServerboundDiagnosticsPacket::create(float avgFps, float avgServerSimTickTimeMS, float avgClientSimTickTimeMS, float avgBeginFrameTimeMS, float avgInputTimeMS, float avgRenderTimeMS, float avgEndFrameTimeMS, float avgRemainderTimePercent, float avgUnaccountedTimePercent, std::vector<types::MemoryCategoryCounter> memoryCategoryValues, std::vector<types::EntityDiagnosticTimingInfo> entityDiagnostics, std::vector<types::SystemDiagnosticTimingInfo> systemDiagnostics, std::vector<types::SystemCategory> systemCategories, std::vector<types::WhiskerScopeDataSummary> whiskerScopes)
{
    ServerboundDiagnosticsPacket result;
    result.avgFps = avgFps;
    result.avgServerSimTickTimeMS = avgServerSimTickTimeMS;
    result.avgClientSimTickTimeMS = avgClientSimTickTimeMS;
    result.avgBeginFrameTimeMS = avgBeginFrameTimeMS;
    result.avgInputTimeMS = avgInputTimeMS;
    result.avgRenderTimeMS = avgRenderTimeMS;
    result.avgEndFrameTimeMS = avgEndFrameTimeMS;
    result.avgRemainderTimePercent = avgRemainderTimePercent;
    result.avgUnaccountedTimePercent = avgUnaccountedTimePercent;
    result.memoryCategoryValues = std::move(memoryCategoryValues);
    result.entityDiagnostics = std::move(entityDiagnostics);
    result.systemDiagnostics = std::move(systemDiagnostics);
    result.systemCategories = std::move(systemCategories);
    result.whiskerScopes = std::move(whiskerScopes);
    return result;
}

void ServerboundDiagnosticsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    avgFps = encoding::LE::readFloat(in);
    avgServerSimTickTimeMS = encoding::LE::readFloat(in);
    avgClientSimTickTimeMS = encoding::LE::readFloat(in);
    avgBeginFrameTimeMS = encoding::LE::readFloat(in);
    avgInputTimeMS = encoding::LE::readFloat(in);
    avgRenderTimeMS = encoding::LE::readFloat(in);
    avgEndFrameTimeMS = encoding::LE::readFloat(in);
    avgRemainderTimePercent = encoding::LE::readFloat(in);
    avgUnaccountedTimePercent = encoding::LE::readFloat(in);

    memoryCategoryValues.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        memoryCategoryValues.push_back(types::MemoryCategoryCounter::read(in));
    }

    entityDiagnostics.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        entityDiagnostics.push_back(types::EntityDiagnosticTimingInfo::read(in));
    }

    systemDiagnostics.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        systemDiagnostics.push_back(types::SystemDiagnosticTimingInfo::read(in));
    }

    systemCategories.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        systemCategories.push_back(types::SystemCategory::read(in));
    }

    whiskerScopes.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        whiskerScopes.push_back(types::WhiskerScopeDataSummary::read(in));
    }

}

void ServerboundDiagnosticsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, avgFps);
    encoding::LE::writeFloat(out, avgServerSimTickTimeMS);
    encoding::LE::writeFloat(out, avgClientSimTickTimeMS);
    encoding::LE::writeFloat(out, avgBeginFrameTimeMS);
    encoding::LE::writeFloat(out, avgInputTimeMS);
    encoding::LE::writeFloat(out, avgRenderTimeMS);
    encoding::LE::writeFloat(out, avgEndFrameTimeMS);
    encoding::LE::writeFloat(out, avgRemainderTimePercent);
    encoding::LE::writeFloat(out, avgUnaccountedTimePercent);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(memoryCategoryValues.size()));
    for (const auto &value : memoryCategoryValues) {
        value.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entityDiagnostics.size()));
    for (const auto &value : entityDiagnostics) {
        value.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(systemDiagnostics.size()));
    for (const auto &value : systemDiagnostics) {
        value.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(systemCategories.size()));
    for (const auto &value : systemCategories) {
        value.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(whiskerScopes.size()));
    for (const auto &value : whiskerScopes) {
        value.write(out);
    }

}

bool ServerboundDiagnosticsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleServerboundDiagnostics(*this);
}

}  // namespace bedrock_protocol
