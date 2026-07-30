/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pmmp/encoding (LE)
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
 * Little-endian fixed-width primitives. This is the default byte order of the Bedrock protocol.
 *
 * Members: readUnsignedShort, readSignedShort, readUnsignedInt, readSignedInt, readUnsignedLong,
 * readSignedLong, readFloat, readDouble, and the matching write* functions.
 */
using LE = detail::FixedWidthCodec<true>;

}  // namespace bedrock_protocol::encoding
