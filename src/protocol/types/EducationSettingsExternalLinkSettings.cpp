/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EducationSettingsExternalLinkSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/EducationSettingsExternalLinkSettings.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

EducationSettingsExternalLinkSettings EducationSettingsExternalLinkSettings::read(encoding::ByteBufferReader &in)
{
    auto url = CommonTypes::getString(in);
    auto displayName = CommonTypes::getString(in);
    // DELIBERATE DIVERGENCE FROM UPSTREAM - the only one in this port.
    //
    // PocketMine's read() (types/EducationSettingsExternalLinkSettings.php) reads url then
    // displayName, then calls `new self($displayName, $url)` against a constructor declared
    // `(string $url, string $displayName)` - so it assigns each value to the other field. write()
    // then emits them in declaration order, and the two strings come out transposed. Every
    // decode-then-encode cycle swaps them, which is exactly what the bridge does whenever a plugin
    // touches this packet.
    //
    // The wire format is defined by Mojang, not by PocketMine, so upstream is simply wrong here.
    // Reproducing the bug verbatim would mean shipping silent data corruption; the 1:1 rule exists to
    // make this port trustworthy, and following it here would do the opposite. If strict upstream
    // parity ever matters more, swap these two arguments back and drop EducationSettingsPacket from
    // the verification selection again.
    return EducationSettingsExternalLinkSettings(std::move(url), std::move(displayName));
}

void EducationSettingsExternalLinkSettings::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, url);
    CommonTypes::putString(out, displayName);
}

}  // namespace bedrock_protocol::types
