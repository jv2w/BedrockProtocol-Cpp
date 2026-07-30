/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/InvalidTagValueException.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <stdexcept>

namespace bedrock_protocol::nbt {

/**
 * Note: this mirrors the PHP hierarchy, where InvalidTagValueException extends
 * \InvalidArgumentException rather than NbtException. It signals a programming error (a value that
 * cannot fit the tag), not corrupt input.
 */
class InvalidTagValueException final : public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
};

}  // namespace bedrock_protocol::nbt
