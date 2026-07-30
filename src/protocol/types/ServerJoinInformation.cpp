/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ServerJoinInformation.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ServerJoinInformation.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

ServerJoinInformation ServerJoinInformation::read(encoding::ByteBufferReader &in)
{
    auto gatheringJoinInfo = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return GatheringJoinInfo::read(in); });
    auto storeEntryPointInfo = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return StoreEntryPointInfo::read(in); });
    auto presenceInfo = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return PresenceInfo::read(in); });

    return ServerJoinInformation(
        std::move(gatheringJoinInfo),
        std::move(storeEntryPointInfo),
        std::move(presenceInfo)
    );
}

void ServerJoinInformation::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(out, gatheringJoinInfo, [](encoding::ByteBufferWriter &out, const GatheringJoinInfo &info) { info.write(out); });
    CommonTypes::writeOptional(out, storeEntryPointInfo, [](encoding::ByteBufferWriter &out, const StoreEntryPointInfo &info) { info.write(out); });
    CommonTypes::writeOptional(out, presenceInfo, [](encoding::ByteBufferWriter &out, const PresenceInfo &info) { info.write(out); });
}

}  // namespace bedrock_protocol::types
