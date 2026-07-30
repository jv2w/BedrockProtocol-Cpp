/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ChangeMobPropertyPacket.php
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

/**
 * This appears to be some kind of debug packet. Does nothing in release mode.
 * I have no words for the structure of this packet ...
 */
class ChangeMobPropertyPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CHANGE_MOB_PROPERTY_PACKET;


    std::int64_t actorUniqueId = 0;
    std::string propertyName;
    bool boolValue = false;
    std::string stringValue;
    std::int32_t intValue = 0;
    float floatValue = 0.0F;

    /**
     * @generate-create-func
     */
    static ChangeMobPropertyPacket create(std::int64_t actorUniqueId, std::string propertyName, bool boolValue, std::string stringValue, std::int32_t intValue, float floatValue);

    /** @note PHP calls this boolValue(); renamed because C++ cannot have a method and a field of the same name. */
    static ChangeMobPropertyPacket makeBoolValue(std::int64_t actorUniqueId, std::string propertyName, bool value);

    /** @note PHP calls this stringValue(); renamed because C++ cannot have a method and a field of the same name. */
    static ChangeMobPropertyPacket makeStringValue(std::int64_t actorUniqueId, std::string propertyName, std::string value);

    /** @note PHP calls this intValue(); renamed because C++ cannot have a method and a field of the same name. */
    static ChangeMobPropertyPacket makeIntValue(std::int64_t actorUniqueId, std::string propertyName, std::int32_t value);

    /** @note PHP calls this floatValue(); renamed because C++ cannot have a method and a field of the same name. */
    static ChangeMobPropertyPacket makeFloatValue(std::int64_t actorUniqueId, std::string propertyName, float value);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ChangeMobPropertyPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
