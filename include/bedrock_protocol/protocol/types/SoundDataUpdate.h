/*
 * This file is part of BedrockProtocol-Cpp.
 * Added for 1.26.40: gophertunnel v1.58.0 minecraft/protocol/sound.go has no PHP counterpart.
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

/**
 * A single change to a sound that is currently playing.
 *
 * gophertunnel v1.58.0 minecraft/protocol/sound.go:13-45. The type is a varuint32 discriminator and
 * decides which of the trailing floats - if any - follow it.
 */
class SoundDataUpdate final {
public:
    static constexpr std::uint32_t TYPE_STOP = 0;
    static constexpr std::uint32_t TYPE_SET_VOLUME = 1;
    static constexpr std::uint32_t TYPE_SET_PITCH = 2;
    static constexpr std::uint32_t TYPE_FADE = 3;
    static constexpr std::uint32_t TYPE_SEEK_TO = 4;
    static constexpr std::uint32_t TYPE_PAUSE = 5;
    static constexpr std::uint32_t TYPE_RESUME = 6;

    SoundDataUpdate() = default;

    SoundDataUpdate(std::uint32_t type, float volume, float pitch, float duration, float targetVolume, float seconds)
        : type(type), volume(volume), pitch(pitch), duration(duration), targetVolume(targetVolume), seconds(seconds)
    {
    }

    [[nodiscard]] std::uint32_t getType() const { return type; }

    /** Used if the type is TYPE_SET_VOLUME. */
    [[nodiscard]] float getVolume() const { return volume; }

    /** Used if the type is TYPE_SET_PITCH. */
    [[nodiscard]] float getPitch() const { return pitch; }

    /** Fade duration in seconds. Used if the type is TYPE_FADE. */
    [[nodiscard]] float getDuration() const { return duration; }

    /** Fade target volume. Used if the type is TYPE_FADE. */
    [[nodiscard]] float getTargetVolume() const { return targetVolume; }

    /** Position to seek to. Used if the type is TYPE_SEEK_TO. */
    [[nodiscard]] float getSeconds() const { return seconds; }

    static SoundDataUpdate read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t type = TYPE_STOP;
    float volume = 0.0f;
    float pitch = 0.0f;
    float duration = 0.0f;
    float targetVolume = 0.0f;
    float seconds = 0.0f;
};

}  // namespace bedrock_protocol::types
