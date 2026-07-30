/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/NoSuchTagException.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/nbt/NbtException.h"

namespace bedrock_protocol::nbt {

class NoSuchTagException : public NbtException {
public:
    using NbtException::NbtException;
};

}  // namespace bedrock_protocol::nbt
