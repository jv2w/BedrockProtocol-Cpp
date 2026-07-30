/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePackStackPacket.php
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
#include "bedrock_protocol/protocol/types/Experiments.h"
#include "bedrock_protocol/protocol/types/resourcepacks/ResourcePackStackEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ResourcePackStackPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::RESOURCE_PACK_STACK_PACKET;


    /** @var ResourcePackStackEntry[] */
    std::vector<types::resourcepacks::ResourcePackStackEntry> resourcePackStack;
    bool mustAccept = false;
    std::string baseGameVersion{ProtocolInfo::MINECRAFT_VERSION_NETWORK};
    types::Experiments experiments;
    bool useVanillaEditorPacks = false;

    /**
     * @generate-create-func
     */
    static ResourcePackStackPacket create(std::vector<types::resourcepacks::ResourcePackStackEntry> resourcePackStack, bool mustAccept, std::string baseGameVersion, types::Experiments experiments, bool useVanillaEditorPacks);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ResourcePackStackPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
