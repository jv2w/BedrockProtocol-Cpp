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

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class PlayerAuthInputVehicleInfo final {
public:
    PlayerAuthInputVehicleInfo(float vehicleRotationX, float vehicleRotationZ,
                               std::int64_t predictedVehicleActorUniqueId)
        : vehicleRotationX(vehicleRotationX), vehicleRotationZ(vehicleRotationZ),
          predictedVehicleActorUniqueId(predictedVehicleActorUniqueId)
    {
    }

    [[nodiscard]] float getVehicleRotationX() const { return vehicleRotationX; }

    [[nodiscard]] float getVehicleRotationZ() const { return vehicleRotationZ; }

    [[nodiscard]] std::int64_t getPredictedVehicleActorUniqueId() const { return predictedVehicleActorUniqueId; }

    static PlayerAuthInputVehicleInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float vehicleRotationX;
    float vehicleRotationZ;
    std::int64_t predictedVehicleActorUniqueId;
};

}  // namespace bedrock_protocol::types
