/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/StartGamePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/uuid/Uuid.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/types/BlockPaletteEntry.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"
#include "bedrock_protocol/protocol/types/LevelSettings.h"
#include "bedrock_protocol/protocol/types/NetworkPermissions.h"
#include "bedrock_protocol/protocol/types/PlayerMovementSettings.h"
#include "bedrock_protocol/protocol/types/ServerJoinInformation.h"
#include "bedrock_protocol/protocol/types/ServerTelemetryData.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class StartGamePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::START_GAME_PACKET;


    std::int64_t actorUniqueId = 0;
    std::uint64_t actorRuntimeId = 0;
    std::int32_t playerGamemode = 0;
    math::Vector3 playerPosition;
    float pitch = 0.0F;
    float yaw = 0.0F;
    /** @phpstan-var CacheableNbt<CompoundTag>  */
    types::CacheableNbt<nbt::tag::CompoundTag> playerActorProperties;  //same as SyncActorPropertyPacket content
    types::LevelSettings levelSettings;
    std::string levelId = "";  //base64 string, usually the same as world folder name in vanilla
    std::string worldName;
    std::string premiumWorldTemplateId = "";
    bool isTrial = false;
    std::optional<types::PlayerMovementSettings> playerMovementSettings;  //@note PHP leaves this uninitialised until decode/create; C++ needs std::optional because the type has no default constructor
    std::uint64_t currentTick = 0;  //only used if isTrial is true
    std::int32_t enchantmentSeed = 0;
    std::string multiplayerCorrelationId = "";  //TODO: this should be filled with a UUID of some sort
    bool enableNewInventorySystem = false;  //TODO
    std::string serverSoftwareVersion;
    uuid::Uuid worldTemplateId;  //why is this here twice ??? mojang
    bool enableClientSideChunkGeneration = false;
    bool blockNetworkIdsAreHashes = false;  //new in 1.19.80, possibly useful for multi version
    std::optional<types::NetworkPermissions> networkPermissions;  //@note see playerMovementSettings
    bool isLoggingChat = false;
    std::optional<types::ServerJoinInformation> serverJoinInformation;
    std::optional<types::ServerTelemetryData> serverTelemetryData;  //@note see playerMovementSettings
    /**
     */
    std::vector<types::BlockPaletteEntry> blockPalette;
    /**
     * Checksum of the full block palette. This is a hash of some weird stringified version of the NBT.
     * This is used along with the baseGameVersion to check for inconsistencies in the block palette.
     * Fill with 0 if you don't want to bother having the client verify the palette (seems pointless anyway).
     */
    std::uint64_t blockPaletteChecksum = 0;

    /**
     * @generate-create-func
     */
    static StartGamePacket create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, std::int32_t playerGamemode, math::Vector3 playerPosition, float pitch, float yaw, types::CacheableNbt<nbt::tag::CompoundTag> playerActorProperties, types::LevelSettings levelSettings, std::string levelId, std::string worldName, std::string premiumWorldTemplateId, bool isTrial, types::PlayerMovementSettings playerMovementSettings, std::uint64_t currentTick, std::int32_t enchantmentSeed, std::string multiplayerCorrelationId, bool enableNewInventorySystem, std::string serverSoftwareVersion, uuid::Uuid worldTemplateId, bool enableClientSideChunkGeneration, bool blockNetworkIdsAreHashes, types::NetworkPermissions networkPermissions, bool isLoggingChat, std::optional<types::ServerJoinInformation> serverJoinInformation, types::ServerTelemetryData serverTelemetryData, std::vector<types::BlockPaletteEntry> blockPalette, std::uint64_t blockPaletteChecksum);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "StartGamePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
