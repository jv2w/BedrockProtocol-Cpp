/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/CommandOutputMessage.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <vector>

namespace bedrock_protocol::types::command {

class CommandOutputMessage {
public:
    bool isInternal = false;
    std::string messageId;
    std::vector<std::string> parameters;
};

}  // namespace bedrock_protocol::types::command
