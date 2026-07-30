/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/NetworkPermissions.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class NetworkPermissions final {
public:
    explicit NetworkPermissions(bool disableClientSounds) : disableClientSounds_(disableClientSounds) {}

    [[nodiscard]] bool disableClientSounds() const
    {
        return disableClientSounds_;
    }

    static NetworkPermissions decode(encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const;

private:
    //the PHP original names the property and its accessor identically; C++ forbids that, so the member takes the
    //infrastructure `member_` suffix while the accessor keeps the PHP name
    bool disableClientSounds_;
};

}  // namespace bedrock_protocol::types
