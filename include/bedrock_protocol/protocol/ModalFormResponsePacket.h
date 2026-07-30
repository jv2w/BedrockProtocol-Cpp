/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ModalFormResponsePacket.php
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

class ModalFormResponsePacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MODAL_FORM_RESPONSE_PACKET;

    static constexpr std::int32_t CANCEL_REASON_CLOSED = 0;
    /** Sent if a form is sent when the player is on a loading screen */
    static constexpr std::int32_t CANCEL_REASON_USER_BUSY = 1;

    std::uint32_t formId = 0;
    std::optional<std::string> formData;  //json
    std::optional<std::uint8_t> cancelReason;

    /**
     * @generate-create-func
     */
    static ModalFormResponsePacket create(std::uint32_t formId, std::optional<std::string> formData, std::optional<std::uint8_t> cancelReason);

    static ModalFormResponsePacket response(std::uint32_t formId, std::optional<std::string> formData);

    static ModalFormResponsePacket cancel(std::uint32_t formId, std::optional<std::uint8_t> cancelReason);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ModalFormResponsePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
