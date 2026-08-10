/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PresenceInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PresenceInfo.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

PresenceInfo PresenceInfo::read(encoding::ByteBufferReader &in)
{
    auto richPresenceId = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CommonTypes::getString(in);
    });

    return PresenceInfo(std::move(richPresenceId));
}

void PresenceInfo::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(out, richPresenceId, [](encoding::ByteBufferWriter &out, const std::string &value) {
        CommonTypes::putString(out, value);
    });
}

}  // namespace bedrock_protocol::types
