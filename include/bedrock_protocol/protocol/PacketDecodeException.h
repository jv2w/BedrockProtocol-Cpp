/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PacketDecodeException.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bedrock_protocol {

/**
 * Thrown when a packet's contents are not valid for the packet type being decoded, e.g. an unknown
 * discriminator value or a field whose length exceeds its documented limit.
 */
class PacketDecodeException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;

    /** Re-labels a lower-level decode failure with the name of the packet that was being decoded. */
    static PacketDecodeException wrap(const std::exception &previous, std::string_view packetName)
    {
        return PacketDecodeException(std::string(packetName) + ": " + previous.what());
    }
};

}  // namespace bedrock_protocol
