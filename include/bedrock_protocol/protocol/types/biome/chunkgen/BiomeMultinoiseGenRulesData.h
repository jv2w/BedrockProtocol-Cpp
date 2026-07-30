/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/biome/chunkgen/BiomeMultinoiseGenRulesData.php
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

namespace bedrock_protocol::types::biome::chunkgen {

class BiomeMultinoiseGenRulesData final {
public:
    BiomeMultinoiseGenRulesData(float temperature, float humidity, float altitude, float weirdness, float weight)
        : temperature(temperature), humidity(humidity), altitude(altitude), weirdness(weirdness), weight(weight)
    {
    }

    [[nodiscard]] float getTemperature() const { return temperature; }

    [[nodiscard]] float getHumidity() const { return humidity; }

    [[nodiscard]] float getAltitude() const { return altitude; }

    [[nodiscard]] float getWeirdness() const { return weirdness; }

    [[nodiscard]] float getWeight() const { return weight; }

    /** @throws DataDecodeException */
    static BiomeMultinoiseGenRulesData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float temperature;
    float humidity;
    float altitude;
    float weirdness;
    float weight;
};

}  // namespace bedrock_protocol::types::biome::chunkgen
