/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayerAuthInputVehicleInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PlayerAuthInputVehicleInfo.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

PlayerAuthInputVehicleInfo PlayerAuthInputVehicleInfo::read(encoding::ByteBufferReader &in)
{
    const auto vehicleRotationX = LE::readFloat(in);
    const auto vehicleRotationZ = LE::readFloat(in);
    const auto predictedVehicleActorUniqueId = CommonTypes::getActorUniqueId(in);

    return PlayerAuthInputVehicleInfo(vehicleRotationX, vehicleRotationZ, predictedVehicleActorUniqueId);
}

void PlayerAuthInputVehicleInfo::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, vehicleRotationX);
    LE::writeFloat(out, vehicleRotationZ);
    CommonTypes::putActorUniqueId(out, predictedVehicleActorUniqueId);
}

}  // namespace bedrock_protocol::types
