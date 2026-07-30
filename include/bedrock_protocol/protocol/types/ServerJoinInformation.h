/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ServerJoinInformation.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/GatheringJoinInfo.h"
#include "bedrock_protocol/protocol/types/PresenceInfo.h"
#include "bedrock_protocol/protocol/types/StoreEntryPointInfo.h"

namespace bedrock_protocol::types {

class ServerJoinInformation final {
public:
    ServerJoinInformation(std::optional<GatheringJoinInfo> gatheringJoinInfo, std::optional<StoreEntryPointInfo> storeEntryPointInfo, std::optional<PresenceInfo> presenceInfo)
        : gatheringJoinInfo(std::move(gatheringJoinInfo)), storeEntryPointInfo(std::move(storeEntryPointInfo)), presenceInfo(std::move(presenceInfo))
    {
    }

    [[nodiscard]] const std::optional<GatheringJoinInfo> &getGatheringJoinInfo() const { return gatheringJoinInfo; }

    [[nodiscard]] const std::optional<StoreEntryPointInfo> &getStoreEntryPointInfo() const { return storeEntryPointInfo; }

    [[nodiscard]] const std::optional<PresenceInfo> &getPresenceInfo() const { return presenceInfo; }

    static ServerJoinInformation read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<GatheringJoinInfo> gatheringJoinInfo;
    std::optional<StoreEntryPointInfo> storeEntryPointInfo;
    std::optional<PresenceInfo> presenceInfo;
};

}  // namespace bedrock_protocol::types
