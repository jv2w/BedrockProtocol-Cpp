/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetTitlePacket.php
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

class SetTitlePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_TITLE_PACKET;

    static constexpr std::int32_t TYPE_CLEAR_TITLE = 0;
    static constexpr std::int32_t TYPE_RESET_TITLE = 1;
    static constexpr std::int32_t TYPE_SET_TITLE = 2;
    static constexpr std::int32_t TYPE_SET_SUBTITLE = 3;
    static constexpr std::int32_t TYPE_SET_ACTIONBAR_MESSAGE = 4;
    static constexpr std::int32_t TYPE_SET_ANIMATION_TIMES = 5;
    static constexpr std::int32_t TYPE_SET_TITLE_JSON = 6;
    static constexpr std::int32_t TYPE_SET_SUBTITLE_JSON = 7;
    static constexpr std::int32_t TYPE_SET_ACTIONBAR_MESSAGE_JSON = 8;

    std::int32_t type = 0;
    std::string text = "";
    std::int32_t fadeInTime = 0;
    std::int32_t stayTime = 0;
    std::int32_t fadeOutTime = 0;
    std::string xuid = "";
    std::string platformOnlineId = "";
    std::string filteredTitleText = "";

    /**
     * @generate-create-func
     */
    static SetTitlePacket create(std::int32_t type, std::string text, std::int32_t fadeInTime, std::int32_t stayTime, std::int32_t fadeOutTime, std::string xuid, std::string platformOnlineId, std::string filteredTitleText);

    static SetTitlePacket title(std::string text);

    static SetTitlePacket subtitle(std::string text);

    static SetTitlePacket actionBarMessage(std::string text);

    static SetTitlePacket clearTitle();

    static SetTitlePacket resetTitleOptions();

    static SetTitlePacket setAnimationTimes(std::int32_t fadeIn, std::int32_t stay, std::int32_t fadeOut);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetTitlePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    /** @note PHP calls this type(); renamed because C++ cannot have a method and a field of the same name. */
    static SetTitlePacket makeType(std::int32_t type);

    /** @note PHP calls this text(); renamed because C++ cannot have a method and a field of the same name. */
    static SetTitlePacket makeText(std::int32_t type, std::string text);

};

}  // namespace bedrock_protocol
