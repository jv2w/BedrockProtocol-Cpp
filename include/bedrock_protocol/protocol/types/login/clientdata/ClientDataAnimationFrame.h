/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/clientdata/ClientDataAnimationFrame.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>

namespace bedrock_protocol::types::login::clientdata {

/**
 * Model class for LoginPacket JSON data for JsonMapper
 */
class ClientDataAnimationFrame final {
public:
    /** @required */
    std::int64_t ImageHeight = 0;

    /** @required */
    std::int64_t ImageWidth = 0;

    /** @required */
    float Frames = 0.0f;

    /** @required */
    std::int64_t Type = 0;

    /** @required */
    std::string Image;

    /** @required */
    std::int64_t AnimationExpression = 0;
};

}  // namespace bedrock_protocol::types::login::clientdata
