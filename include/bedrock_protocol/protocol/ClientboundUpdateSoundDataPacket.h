/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundUpdateSoundDataPacket.php
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
#include "bedrock_protocol/protocol/types/SoundDataUpdate.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientboundUpdateSoundDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENTBOUND_UPDATE_SOUND_DATA_PACKET;


    std::uint64_t serverSoundHandle = 0;
    /**
     * Seven independent union slots, each of which may hold ANY SoundDataUpdate variant - the slot
     * name does not constrain the variant on the wire.
     * gophertunnel v1.58.0 minecraft/protocol/packet/clientbound_update_sound_data.go:11-17.
     */
    std::optional<types::SoundDataUpdate> stop = std::nullopt;
    std::optional<types::SoundDataUpdate> setVolume = std::nullopt;
    std::optional<types::SoundDataUpdate> setPitch = std::nullopt;
    std::optional<types::SoundDataUpdate> fade = std::nullopt;
    std::optional<types::SoundDataUpdate> seekTo = std::nullopt;
    std::optional<types::SoundDataUpdate> pause = std::nullopt;
    std::optional<types::SoundDataUpdate> resume = std::nullopt;

    /**
     * @generate-create-func
     */
    static ClientboundUpdateSoundDataPacket create(std::uint64_t serverSoundHandle, std::optional<types::SoundDataUpdate> stop, std::optional<types::SoundDataUpdate> setVolume, std::optional<types::SoundDataUpdate> setPitch, std::optional<types::SoundDataUpdate> fade, std::optional<types::SoundDataUpdate> seekTo, std::optional<types::SoundDataUpdate> pause, std::optional<types::SoundDataUpdate> resume);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientboundUpdateSoundDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
