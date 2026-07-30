/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ResourcePacksInfoPacket.php
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
#include "bedrock_protocol/uuid/Uuid.h"
#include "bedrock_protocol/protocol/types/resourcepacks/ResourcePackInfoEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ResourcePacksInfoPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::RESOURCE_PACKS_INFO_PACKET;


    /** @var ResourcePackInfoEntry[] */
    std::vector<types::resourcepacks::ResourcePackInfoEntry> resourcePackEntries;
    bool mustAccept = false;  //if true, forces client to choose between accepting packs or being disconnected
    bool hasAddons = false;
    bool hasScripts = false;  //if true, causes disconnect for any platform that doesn't support scripts yet
    uuid::Uuid worldTemplateId;
    std::string worldTemplateVersion;
    bool forceDisableVibrantVisuals = false;

    /**
     * @generate-create-func
     */
    static ResourcePacksInfoPacket create(std::vector<types::resourcepacks::ResourcePackInfoEntry> resourcePackEntries, bool mustAccept, bool hasAddons, bool hasScripts, uuid::Uuid worldTemplateId, std::string worldTemplateVersion, bool forceDisableVibrantVisuals);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ResourcePacksInfoPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
