/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PacketPool.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string_view>

#include "bedrock_protocol/protocol/DataPacket.h"

namespace bedrock_protocol {

/**
 * Registry mapping network IDs to the packet classes that implement them.
 *
 * PHP stores one prototype instance per ID and decodes into it; that is not safe in C++ (the
 * instances would be shared mutable state), so this port stores a factory function per ID and
 * hands out a fresh instance on every lookup.
 */
class PacketPool {
public:
    using PacketFactory = std::unique_ptr<DataPacket> (*)();

    /** Returns the pool holding every packet known to this protocol version. */
    static const PacketPool &getInstance();

    /** Returns a new instance of the packet with the given network ID, or nullptr if it is unknown. */
    [[nodiscard]] std::unique_ptr<DataPacket> getPacketById(std::uint32_t pid) const;

    /**
     * Returns a new instance of the packet whose header begins the given buffer, or nullptr if the
     * ID is unknown. The buffer is not consumed; only the header VarInt is peeked at.
     */
    [[nodiscard]] std::unique_ptr<DataPacket> getPacket(std::string_view buffer) const;

    /** Registers a packet class under its own NETWORK_ID, replacing any previous registration. */
    template <typename T>
    void registerPacket()
    {
        pool_[T::NETWORK_ID] = []() -> std::unique_ptr<DataPacket> { return std::make_unique<T>(); };
    }

private:
    static constexpr std::size_t POOL_SIZE = 512;

    PacketPool();

    std::array<PacketFactory, POOL_SIZE> pool_{};
};

}  // namespace bedrock_protocol
