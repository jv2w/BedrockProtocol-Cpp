/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SystemDiagnosticTimingInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SystemDiagnosticTimingInfo.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

SystemDiagnosticTimingInfo SystemDiagnosticTimingInfo::read(encoding::ByteBufferReader &in)
{
    auto displayName = CommonTypes::getString(in);
    const auto systemIndex = LE::readUnsignedLong(in);
    const auto timeInNS = LE::readUnsignedLong(in);
    const auto percentOfTotal = Byte::readUnsigned(in);

    return SystemDiagnosticTimingInfo(std::move(displayName), systemIndex, timeInNS, percentOfTotal);
}

void SystemDiagnosticTimingInfo::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, displayName);
    LE::writeUnsignedLong(out, systemIndex);
    LE::writeUnsignedLong(out, timeInNS);
    Byte::writeUnsigned(out, percentOfTotal);
}

}  // namespace bedrock_protocol::types
