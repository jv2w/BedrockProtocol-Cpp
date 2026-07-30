/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pmmp/encoding (DataDecodeException)
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace bedrock_protocol::encoding {

/**
 * Thrown when the raw binary layer cannot satisfy a read request, e.g. because the buffer ran out
 * of bytes or a VarInt did not terminate within its maximum length.
 *
 * This is the low-level counterpart to PacketDecodeException; packet-level decode paths wrap it.
 */
class DataDecodeException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

}  // namespace bedrock_protocol::encoding
