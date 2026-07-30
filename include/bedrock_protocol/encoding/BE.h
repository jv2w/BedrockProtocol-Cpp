/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pmmp/encoding (BE)
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/encoding/FixedWidthCodec.h"

namespace bedrock_protocol::encoding {

/**
 * Big-endian fixed-width primitives. Rarely used by the Bedrock protocol; LoginPacket's protocol
 * version field and the RakNet-adjacent parts are the notable exceptions.
 */
using BE = detail::FixedWidthCodec<false>;

}  // namespace bedrock_protocol::encoding
