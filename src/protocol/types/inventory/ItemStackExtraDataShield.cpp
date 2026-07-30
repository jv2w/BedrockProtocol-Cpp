/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ItemStackExtraDataShield.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/ItemStackExtraDataShield.h"

#include <utility>

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types::inventory {

using encoding::LE;

ItemStackExtraDataShield ItemStackExtraDataShield::read(encoding::ByteBufferReader &in)
{
    auto base = ItemStackExtraData::read(in);
    //TODO: I don't know for sure if this is supposed to be signed or unsigned
    const auto blockingTick = LE::readSignedLong(in);

    return {base.getNbt(), base.getCanPlaceOn(), base.getCanDestroy(), blockingTick};
}

void ItemStackExtraDataShield::write(encoding::ByteBufferWriter &out) const
{
    ItemStackExtraData::write(out);
    LE::writeSignedLong(out, blockingTick);
}

}  // namespace bedrock_protocol::types::inventory
