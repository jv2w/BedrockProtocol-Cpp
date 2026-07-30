/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/resourcepacks/ResourcePackInfoEntry.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::resourcepacks {

class ResourcePackInfoEntry {
public:
    ResourcePackInfoEntry(uuid::Uuid packId, std::string version, std::uint64_t sizeBytes,
                          std::string encryptionKey = "", std::string subPackName = "", std::string contentId = "",
                          bool hasScripts = false, bool isAddonPack = false, bool isRtxCapable = false,
                          std::string cdnUrl = "")
        : packId(packId), version(std::move(version)), sizeBytes(sizeBytes), encryptionKey(std::move(encryptionKey)),
          subPackName(std::move(subPackName)), contentId(std::move(contentId)), hasScripts_(hasScripts),
          isAddonPack_(isAddonPack), isRtxCapable_(isRtxCapable), cdnUrl(std::move(cdnUrl))
    {
    }

    virtual ~ResourcePackInfoEntry() = default;

    [[nodiscard]] const uuid::Uuid &getPackId() const { return packId; }

    [[nodiscard]] const std::string &getVersion() const { return version; }

    [[nodiscard]] std::uint64_t getSizeBytes() const { return sizeBytes; }

    [[nodiscard]] const std::string &getEncryptionKey() const { return encryptionKey; }

    [[nodiscard]] const std::string &getSubPackName() const { return subPackName; }

    [[nodiscard]] const std::string &getContentId() const { return contentId; }

    [[nodiscard]] bool hasScripts() const { return hasScripts_; }

    [[nodiscard]] bool isAddonPack() const { return isAddonPack_; }

    [[nodiscard]] bool isRtxCapable() const { return isRtxCapable_; }

    [[nodiscard]] const std::string &getCdnUrl() const { return cdnUrl; }

    void write(encoding::ByteBufferWriter &out) const;

    static ResourcePackInfoEntry read(encoding::ByteBufferReader &in);

private:
    uuid::Uuid packId;
    std::string version;
    std::uint64_t sizeBytes;
    std::string encryptionKey;
    std::string subPackName;
    std::string contentId;
    bool hasScripts_;    // renamed: PHP field $hasScripts collides with the hasScripts() getter in C++
    bool isAddonPack_;   // renamed: PHP field $isAddonPack collides with the isAddonPack() getter in C++
    bool isRtxCapable_;  // renamed: PHP field $isRtxCapable collides with the isRtxCapable() getter in C++
    std::string cdnUrl;
};

}  // namespace bedrock_protocol::types::resourcepacks
