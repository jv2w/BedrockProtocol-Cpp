/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/EducationEditionOffer.php
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

struct EducationEditionOffer final {
    EducationEditionOffer() = delete;
    //NOOP

    static constexpr std::int32_t NONE = 0;
    static constexpr std::int32_t EVERYWHERE_EXCEPT_CHINA = 1;
    static constexpr std::int32_t CHINA = 2;
};

}  // namespace bedrock_protocol::types
