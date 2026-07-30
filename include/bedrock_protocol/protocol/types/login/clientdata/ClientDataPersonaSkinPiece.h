/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/clientdata/ClientDataPersonaSkinPiece.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>

namespace bedrock_protocol::types::login::clientdata {

/**
 * Model class for LoginPacket JSON data for JsonMapper
 */
class ClientDataPersonaSkinPiece final {
public:
    /** @required */
    std::string PieceId;

    /** @required */
    std::string PieceType;

    /** @required */
    std::string PackId;

    /** @required */
    bool IsDefault = false;

    /** @required */
    std::string ProductId;
};

}  // namespace bedrock_protocol::types::login::clientdata
