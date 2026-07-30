/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/EditorWorldType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

struct EditorWorldType final {
    EditorWorldType() = delete;
    //NOOP

    static constexpr std::int32_t NON_EDITOR = 0;
    static constexpr std::int32_t PROJECT = 1;
    static constexpr std::int32_t TEST_LEVEL = 2;
    static constexpr std::int32_t REALMS_UPLOAD = 3;
};

}  // namespace bedrock_protocol::types
