/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PhotoTransferPacket.php
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

namespace bedrock_protocol {

class PacketHandlerInterface;

class PhotoTransferPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PHOTO_TRANSFER_PACKET;


    std::string photoName;
    std::string photoData;
    std::string bookId;  //photos are stored in a sibling directory to the games folder (screenshots/(some UUID)/bookID/example.png)
    std::uint8_t type = 0;
    std::uint8_t sourceType = 0;
    std::int64_t ownerActorUniqueId = 0;
    std::string newPhotoName;  //???

    /**
     * @generate-create-func
     */
    static PhotoTransferPacket create(std::string photoName, std::string photoData, std::string bookId, std::uint8_t type, std::uint8_t sourceType, std::int64_t ownerActorUniqueId, std::string newPhotoName);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PhotoTransferPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
