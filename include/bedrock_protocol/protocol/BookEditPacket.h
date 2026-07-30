/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BookEditPacket.php
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

class BookEditPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::BOOK_EDIT_PACKET;

    static constexpr std::int32_t TYPE_REPLACE_PAGE = 0;
    static constexpr std::int32_t TYPE_ADD_PAGE = 1;
    static constexpr std::int32_t TYPE_DELETE_PAGE = 2;
    static constexpr std::int32_t TYPE_SWAP_PAGES = 3;
    static constexpr std::int32_t TYPE_SIGN_BOOK = 4;

    std::uint32_t type = 0;
    std::int32_t inventorySlot = 0;
    std::int32_t pageNumber = 0;
    std::int32_t secondaryPageNumber = 0;
    std::string text;
    std::string photoName;
    std::string title;
    std::string author;
    std::string xuid;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "BookEditPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
