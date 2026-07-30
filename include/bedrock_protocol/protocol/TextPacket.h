/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/TextPacket.php
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

class TextPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::TEXT_PACKET;

    static constexpr std::int32_t TYPE_RAW = 0;
    static constexpr std::int32_t TYPE_CHAT = 1;
    static constexpr std::int32_t TYPE_TRANSLATION = 2;
    static constexpr std::int32_t TYPE_POPUP = 3;
    static constexpr std::int32_t TYPE_JUKEBOX_POPUP = 4;
    static constexpr std::int32_t TYPE_TIP = 5;
    static constexpr std::int32_t TYPE_SYSTEM = 6;
    static constexpr std::int32_t TYPE_WHISPER = 7;
    static constexpr std::int32_t TYPE_ANNOUNCEMENT = 8;
    static constexpr std::int32_t TYPE_JSON_WHISPER = 9;
    static constexpr std::int32_t TYPE_JSON = 10;
    static constexpr std::int32_t TYPE_JSON_ANNOUNCEMENT = 11;

    std::uint8_t type = 0;
    bool needsTranslation = false;
    std::string sourceName;
    std::string message;
    /** @var string[] */
    std::vector<std::string> parameters;
    std::string xboxUserId = "";
    std::string platformChatId = "";
    std::optional<std::string> filteredMessage = std::nullopt;

    static TextPacket raw(const std::string &message);

    /**
     * @param parameters
     */
    static TextPacket translation(const std::string &key, const std::vector<std::string> &parameters = {});

    static TextPacket popup(const std::string &message);

    /**
     * @param parameters
     */
    static TextPacket translatedPopup(const std::string &key, const std::vector<std::string> &parameters = {});

    /**
     * @param parameters
     */
    static TextPacket jukeboxPopup(const std::string &key, const std::vector<std::string> &parameters = {});

    static TextPacket tip(const std::string &message);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "TextPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    static TextPacket messageOnly(std::uint8_t type, const std::string &message);

    /**
     * @param parameters
     */
    static TextPacket baseTranslation(std::uint8_t type, const std::string &key, const std::vector<std::string> &parameters);

    static constexpr std::int32_t CATEGORY_MESSAGE_ONLY = 0;
    static constexpr std::int32_t CATEGORY_AUTHORED_MESSAGE = 1;
    static constexpr std::int32_t CATEGORY_MESSAGE_WITH_PARAMETERS = 2;
};

}  // namespace bedrock_protocol
