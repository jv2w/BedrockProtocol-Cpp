/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/StartGamePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/StartGamePacket.h"

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

StartGamePacket StartGamePacket::create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, std::int32_t playerGamemode, math::Vector3 playerPosition, float pitch, float yaw, types::CacheableNbt<nbt::tag::CompoundTag> playerActorProperties, types::LevelSettings levelSettings, std::string levelId, std::string worldName, std::string premiumWorldTemplateId, bool isTrial, types::PlayerMovementSettings playerMovementSettings, std::uint64_t currentTick, std::int32_t enchantmentSeed, std::string multiplayerCorrelationId, bool enableNewInventorySystem, std::string serverSoftwareVersion, uuid::Uuid worldTemplateId, bool enableClientSideChunkGeneration, bool blockNetworkIdsAreHashes, types::NetworkPermissions networkPermissions, bool isLoggingChat, std::optional<types::ServerJoinInformation> serverJoinInformation, types::ServerTelemetryData serverTelemetryData, std::vector<types::BlockPaletteEntry> blockPalette, std::uint64_t blockPaletteChecksum)
{
    StartGamePacket result;
    result.actorUniqueId = actorUniqueId;
    result.actorRuntimeId = actorRuntimeId;
    result.playerGamemode = playerGamemode;
    result.playerPosition = std::move(playerPosition);
    result.pitch = pitch;
    result.yaw = yaw;
    result.playerActorProperties = std::move(playerActorProperties);
    result.levelSettings = std::move(levelSettings);
    result.levelId = std::move(levelId);
    result.worldName = std::move(worldName);
    result.premiumWorldTemplateId = std::move(premiumWorldTemplateId);
    result.isTrial = isTrial;
    result.playerMovementSettings = std::move(playerMovementSettings);
    result.currentTick = currentTick;
    result.enchantmentSeed = enchantmentSeed;
    result.multiplayerCorrelationId = std::move(multiplayerCorrelationId);
    result.enableNewInventorySystem = enableNewInventorySystem;
    result.serverSoftwareVersion = std::move(serverSoftwareVersion);
    result.worldTemplateId = std::move(worldTemplateId);
    result.enableClientSideChunkGeneration = enableClientSideChunkGeneration;
    result.blockNetworkIdsAreHashes = blockNetworkIdsAreHashes;
    result.networkPermissions = std::move(networkPermissions);
    result.isLoggingChat = isLoggingChat;
    result.serverJoinInformation = std::move(serverJoinInformation);
    result.serverTelemetryData = std::move(serverTelemetryData);
    result.blockPalette = std::move(blockPalette);
    result.blockPaletteChecksum = blockPaletteChecksum;
    return result;
}

void StartGamePacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    playerGamemode = encoding::VarInt::readSignedInt(in);

    playerPosition = serializer::CommonTypes::getVector3(in);

    pitch = encoding::LE::readFloat(in);
    yaw = encoding::LE::readFloat(in);

    levelSettings = types::LevelSettings::read(in);

    levelId = serializer::CommonTypes::getString(in);
    worldName = serializer::CommonTypes::getString(in);
    premiumWorldTemplateId = serializer::CommonTypes::getString(in);
    isTrial = serializer::CommonTypes::getBool(in);
    playerMovementSettings = types::PlayerMovementSettings::read(in);
    currentTick = encoding::LE::readUnsignedLong(in);

    enchantmentSeed = encoding::VarInt::readSignedInt(in);

    blockPalette.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        const auto blockName = serializer::CommonTypes::getString(in);
        const auto state = serializer::CommonTypes::getNbtCompoundRoot(in);
        blockPalette.push_back(types::BlockPaletteEntry(blockName, types::CacheableNbt<nbt::tag::CompoundTag>(state)));
    }

    multiplayerCorrelationId = serializer::CommonTypes::getString(in);
    enableNewInventorySystem = serializer::CommonTypes::getBool(in);
    serverSoftwareVersion = serializer::CommonTypes::getString(in);
    playerActorProperties = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));
    blockPaletteChecksum = encoding::LE::readUnsignedLong(in);
    worldTemplateId = serializer::CommonTypes::getUUID(in);
    enableClientSideChunkGeneration = serializer::CommonTypes::getBool(in);
    blockNetworkIdsAreHashes = serializer::CommonTypes::getBool(in);
    networkPermissions = types::NetworkPermissions::decode(in);
    isLoggingChat = serializer::CommonTypes::getBool(in);
    serverJoinInformation = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::ServerJoinInformation::read(reader); });
    serverTelemetryData = types::ServerTelemetryData::read(in);

}

void StartGamePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeSignedInt(out, playerGamemode);

    serializer::CommonTypes::putVector3(out, playerPosition);

    encoding::LE::writeFloat(out, pitch);
    encoding::LE::writeFloat(out, yaw);

    levelSettings.write(out);

    serializer::CommonTypes::putString(out, levelId);
    serializer::CommonTypes::putString(out, worldName);
    serializer::CommonTypes::putString(out, premiumWorldTemplateId);
    serializer::CommonTypes::putBool(out, isTrial);
    playerMovementSettings->write(out);
    encoding::LE::writeUnsignedLong(out, currentTick);

    encoding::VarInt::writeSignedInt(out, enchantmentSeed);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(blockPalette.size()));
    for (const auto &entry : blockPalette) {
        serializer::CommonTypes::putString(out, entry.getName());
        out.writeByteArray(entry.getStates().getEncodedNbt());
    }

    serializer::CommonTypes::putString(out, multiplayerCorrelationId);
    serializer::CommonTypes::putBool(out, enableNewInventorySystem);
    serializer::CommonTypes::putString(out, serverSoftwareVersion);
    out.writeByteArray(playerActorProperties.getEncodedNbt());
    encoding::LE::writeUnsignedLong(out, blockPaletteChecksum);
    serializer::CommonTypes::putUUID(out, worldTemplateId);
    serializer::CommonTypes::putBool(out, enableClientSideChunkGeneration);
    serializer::CommonTypes::putBool(out, blockNetworkIdsAreHashes);
    networkPermissions->encode(out);
    serializer::CommonTypes::putBool(out, isLoggingChat);
    serializer::CommonTypes::writeOptional(out, serverJoinInformation, [](encoding::ByteBufferWriter &writer, const types::ServerJoinInformation &info) { info.write(writer); });
    serverTelemetryData->write(out);

}

bool StartGamePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleStartGame(*this);
}

}  // namespace bedrock_protocol
