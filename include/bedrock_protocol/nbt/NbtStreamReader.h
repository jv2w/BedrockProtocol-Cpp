/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/NbtStreamReader.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace bedrock_protocol::nbt {

/**
 * @internal
 */
class NbtStreamReader {
public:
    virtual ~NbtStreamReader() = default;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::uint8_t readByte() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::int8_t readSignedByte() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::uint16_t readShort() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::int16_t readSignedShort() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::int32_t readInt() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::int64_t readLong() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual float readFloat() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual double readDouble() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::string readByteArray() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::string readString() = 0;

    /**
     * @throws encoding::DataDecodeException
     */
    virtual std::vector<std::int32_t> readIntArray() = 0;
};

}  // namespace bedrock_protocol::nbt
